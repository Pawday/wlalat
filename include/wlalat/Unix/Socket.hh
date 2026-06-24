#pragma once

#include "ClosableFD.hh"
#include "wlalat/Types.hh"

#include <algorithm>
#include <wlalat/Error.hh>
#include <wlalat/Message.hh>
#include <wlalat/MessageParser.hh>
#include <wlalat/MessageSerializer.hh>
#include <wlalat/MessageViewFD.hh>

#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <unistd.h>

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <array>
#include <format>
#include <memory_resource>
#include <optional>
#include <span>
#include <utility>
#include <vector>

namespace wlalat
{

namespace Unix
{

struct Socket
{
    static sockaddr_un make_address(uid_t user_id, uint32_t server_id)
    {
        sockaddr_un o{};
        o.sun_family = PF_UNIX;
        std::span<char> path{o.sun_path};
        path = path.subspan(0, path.size() - 1);
        std::format_to_n(
            path.begin(),
            path.size(),
            "/run/user/{}/wayland-{}",
            user_id,
            server_id);
        return o;
    }

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

  private:
    void priv_send(std::span<const std::byte> to_send, std::span<const int> fds)
    {
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
        for (auto &fd : fds) {
            cmsghdr &cmsg = *cmsg_p;
            cmsg = cmsghdr{};
            cmsg.cmsg_level = SOL_SOCKET;
            cmsg.cmsg_type = SCM_RIGHTS;
            cmsg.cmsg_len = CMSG_LEN(sizeof(fd));
            void *cmsg_payload = CMSG_DATA(cmsg_p);
            std::memcpy(cmsg_payload, &fd, sizeof(fd));
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

  public:
    [[deprecated("Removing MessageView")]] void send(MessageViewFD<int> msg)
    {
        auto to_send = _send_serializer(msg);
        priv_send(to_send, msg.fds);
    }

    template <typename MessageT>
    void send(Object obj, const MessageT &msg, std::span<int> fds = {})
    {
        auto to_send = _send_serializer(obj, msg);
        priv_send(to_send, fds);
    }

    [[deprecated("Removing MessageView")]] std::optional<MessageView> recv()
    {
        return recv_at(_recv_buffer);
    }

  private:
    std::optional<MessageView> recv_at(std::pmr::vector<std::byte> &buf)
    {
        std::array<std::byte, 8> header{};
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
            throw Error::from_cstring("No header");
        }

        MessageParser header_view{header};
        auto proto_size_op = header_view.size();
        if (!proto_size_op) {
            _fd.close();
            throw Error::from_cstring("Bad header");
        }
        uint16_t proto_size = proto_size_op.value();
        if (proto_size < header.size()) {
            _fd.close();
            throw Error::from_cstring("Bad size in header");
        }

        if (buf.size() < proto_size) {
            buf.resize(proto_size);
        }

        std::span<std::byte> message_data{buf};
        message_data = message_data.subspan(0, proto_size);

        status = ::recv(_fd(), message_data.data(), message_data.size(), 0);
        if (status < 0) {
            int err = errno;
            _fd.close();
            throw Error::from_errno(err);
        }

        if (status != proto_size) {
            _fd.close();
            throw Error::from_cstring("No message");
        }

        MessageParser parser{message_data};
        return parser.try_parse();
    }

    ClosableFD _fd;
    MessageSerializer _send_serializer;
    std::pmr::vector<std::byte> _recv_buffer;
    std::pmr::vector<std::byte> _ancillary_send_buffer;
};

} // namespace Unix
} // namespace wlalat
