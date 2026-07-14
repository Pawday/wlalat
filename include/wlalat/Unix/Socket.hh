#pragma once

#include "ClosableFD.hh"

#include <wlalat/Binary.hh>
#include <wlalat/Error.hh>
#include <wlalat/MessageHeader.hh>
#include <wlalat/MessageSerializer.hh>
#include <wlalat/Parser.hh>
#include <wlalat/Traits.hh>
#include <wlalat/Types.hh>
#include <wlalat/Unix/TypeTags.hh>

#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <unistd.h>

#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <array>
#include <expected>
#include <format>
#include <memory_resource>
#include <optional>
#include <span>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace wlalat
{

namespace Unix
{

struct Socket
{
    static std::optional<sockaddr_un> try_make_address_default()
    {
        sockaddr_un o{};
        o.sun_family = PF_UNIX;
        std::span<char> path{o.sun_path};
        path = path.subspan(0, path.size() - 1);

        const char *XDG_RUNTIME_DIR = getenv("XDG_RUNTIME_DIR");
        if (!XDG_RUNTIME_DIR) {
            throw Error::from_cstring("Missing XDG_RUNTIME_DIR env");
        }

        const char *WAYLAND_DISPLAY = getenv("WAYLAND_DISPLAY");
        if (!WAYLAND_DISPLAY) {
            throw Error::from_cstring("Missing WAYLAND_DISPLAY env");
        }

        size_t size =
            std::formatted_size("{}/{}", XDG_RUNTIME_DIR, WAYLAND_DISPLAY);

        if (size > path.size() - 1) {
            const char *msg = "Bad XDG_RUNTIME_DIR and WAYLAND_DISPLAY"
                              " (doesn't fit to sockaddr_un::sun_family)";
            throw Error::from_cstring(msg);
        }

        std::format_to(path.begin(), "{}/{}", XDG_RUNTIME_DIR, WAYLAND_DISPLAY);
        return o;
    }

    static sockaddr_un make_address_default()
    {
        return try_make_address_default().value();
    }

    Socket(sockaddr_un addr, std::pmr::memory_resource *res)
        : _send_serializer{res}, _recv_buffer{res}, _ancillary_send_buffer{res}
    {
        ClosableFD fd;
        {
            int raw_fd = ::socket(PF_UNIX, SOCK_STREAM, 0);
            if (raw_fd < 0) {
                int err = errno;
                throw Error::from_errno(err);
            }
            fd = ClosableFD{raw_fd};
        }

        sockaddr *addr_p = reinterpret_cast<sockaddr *>(&addr);
        int status = ::connect(fd(), addr_p, sizeof(addr));
        if (status != 0) {
            int err = errno;
            throw Error::from_errno(err);
        }

        std::swap(_fd, fd);
    }

    Socket() : Socket{make_address_default(), std::pmr::get_default_resource()}
    {
    }

    template <typename MessageT>
    void send(Object obj, const MessageT &msg)
    {
        auto ser = _send_serializer(obj, msg);
        send_raw(ser.message, ser.fds);
    }

    void send_raw(std::span<const std::byte> data, std::span<const void *> fds)
    {
        auto to_send = data;

        auto ancillary_size = CMSG_SPACE(sizeof(int)) * fds.size();
        if (ancillary_size > _ancillary_send_buffer.size()) {
            _ancillary_send_buffer.resize(ancillary_size);
        }
        std::ranges::fill(_ancillary_send_buffer, std::byte{0});

        msghdr sendmsg_arg{};

        iovec to_send_iov{};
        /* ୧༼ಠ益ಠ༽︻╦╤─ */
        to_send_iov.iov_base = const_cast<std::byte *>(to_send.data());
        to_send_iov.iov_len = to_send.size();
        sendmsg_arg.msg_iovlen = 1;
        sendmsg_arg.msg_iov = &to_send_iov;

        sendmsg_arg.msg_controllen = ancillary_size;
        sendmsg_arg.msg_control = _ancillary_send_buffer.data();

        cmsghdr *cmsg_p = CMSG_FIRSTHDR(&sendmsg_arg);
        for (const void *fd_p : fds) {
            const int *fd = static_cast<const int *>(fd_p);
            cmsghdr &cmsg = *cmsg_p;
            cmsg = cmsghdr{};
            cmsg.cmsg_level = SOL_SOCKET;
            cmsg.cmsg_type = SCM_RIGHTS;
            cmsg.cmsg_len = CMSG_LEN(sizeof(fd));
            void *cmsg_payload = CMSG_DATA(cmsg_p);
            std::memcpy(cmsg_payload, fd, sizeof(int));
            cmsg_p = CMSG_NXTHDR(&sendmsg_arg, cmsg_p);
        }

        int status = ::sendmsg(_fd(), &sendmsg_arg, MSG_NOSIGNAL);
        if (status < 0) {
            int err = errno;
            _fd.close();
            throw Error::from_errno(err);
        }

        if (status != to_send.size()) {
            _fd.close();
            throw Error::from_cstring("Send size failure");
        }
    }

    std::optional<MessageHeader> peek_header()
    {
        std::array<std::byte, 8> header{};
        auto header_data_span = std::span{header};
        int status = ::recv(
            _fd(), header.data(), header.size(), MSG_DONTWAIT | MSG_PEEK);
        int err = errno;
        if (status < 0 && err == EWOULDBLOCK) {
            return {};
        }
        if (status < 0) {
            _fd.close();
            throw Error::from_errno(err);
        }

        if (status != header.size()) {
            _fd.close();
            throw Error::from_cstring("No header data");
        }

        auto object_id = fle32(header_data_span.subspan<0, 4>());
        auto size_opcode_pair = fle32(header_data_span.subspan<4, 4>());
        auto size = (size_opcode_pair >> 16) & 0xffff;
        auto opcode = size_opcode_pair & 0xffff;

        if (size < header.size()) {
            _fd.close();
            throw Error::from_cstring(
                "Protocol violation: Message size value from header is less "
                "than 8 (minimal message size)");
        }

        MessageHeader O{};
        O.object_id = object_id;
        O.opcode = opcode;
        O.size = size;
        return O;
    }

    template <typename EventMessageT>
    std::expected<EventMessageT, std::string>
        recv_event(std::type_identity<EventMessageT> ev_t)
    {
        auto H_op = peek_header();
        if (!H_op) {
            return std::unexpected{"No header"};
        }
        auto &H = H_op.value();

        if (_recv_buffer.size() < H.size) {
            _recv_buffer.resize(H.size);
        }

        std::span<std::byte> message_data{_recv_buffer};
        message_data = message_data.subspan(0, H.size);

        int status = ::recv(_fd(), message_data.data(), message_data.size(), 0);
        if (status < 0) {
            int err = errno;
            _fd.close();
            return std::unexpected{strerror(err)};
        }

        if (status != H.size) {
            _fd.close();
            return std::unexpected{"Bad message"};
        }

        auto payload = message_data.subspan(8);
        wlalat::Parser P{payload};
        EventMessageT M{};
        bool good = true;

        using MsgTraits = wlalat::Traits<EventMessageT>;
        auto &metas = MsgTraits::template args_meta<WlTags>;
        auto F = [&](auto... meta) {
            ((good = good && P(M.*(std::get<1>(meta)), std::get<0>(meta))),
             ...);
        };
        std::apply(F, metas);
        if (!good) {
            return std::unexpected{"Parse failure"};
        }

        return M;
    }

  private:
    ClosableFD _fd;
    MessageSerializer _send_serializer;
    std::pmr::vector<std::byte> _recv_buffer;
    std::pmr::vector<std::byte> _ancillary_send_buffer;
};

} // namespace Unix
} // namespace wlalat
