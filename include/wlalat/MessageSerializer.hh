#pragma once

#include "Binary.hh"
#include "Error.hh"
#include "Traits.hh"
#include "Types.hh"
#include "Writer.hh"

#include "Unix/TypeTags.hh"

#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <iterator>
#include <memory>
#include <memory_resource>
#include <span>
#include <tuple>
#include <variant>
#include <vector>

namespace wlalat
{

struct MessageSerializer
{
    MessageSerializer() = default;
    MessageSerializer(std::pmr::memory_resource *res) : data{res}, fds{res}
    {
    }

    struct WriteOutputData
    {
        std::span<const std::byte> message;
        std::span<const void *> fds;
    };

    template <typename MessageT>
    WriteOutputData operator()(Object object_id, const MessageT &msg)
    {
        data.clear();
        fds.clear();
        auto oiter = std::back_inserter(data);

        uint_least64_t message_size = 0;
        OIterSize size_counter{message_size};
        Writer size_w{size_counter};
        InterceptFDWriter fd_itercept_W{size_w, nullptr};
        write_args(msg, fd_itercept_W);

        message_size += 8;

        if (message_size > 0xffff) {
            throw Error::from_cstring("payload is to big");
        }

        auto msg_id_data = tole32(object_id.raw());
        std::ranges::copy(msg_id_data, oiter);

        uint32_t size_opcode_pair = 0;
        size_opcode_pair |= message_size;
        size_opcode_pair <<= 16;
        size_opcode_pair |= MessageT::Meta::opcode;
        auto size_opcode_pair_data = tole32(size_opcode_pair);
        std::ranges::copy(size_opcode_pair_data, oiter);

        Writer W{oiter};
        InterceptFDWriter fd_itercept_W_write{W, std::addressof(fds)};
        write_args(msg, fd_itercept_W_write);

        std::span<const std::byte> data_span{data};
        data_span = data_span.subspan(0, message_size);

        WriteOutputData O{};
        O.message = data_span;
        O.fds = fds;
        return O;
    }

  private:
    struct OIterSize
    {
        using difference_type = std::ptrdiff_t;
        using value_type = void;

        constexpr OIterSize(uint_least64_t &o_size) : _count{&o_size}
        {
        }

        template <typename T>
        constexpr OIterSize &operator=(const T &)
        {
            ++(*_count);
            return *this;
        }

        constexpr OIterSize &operator*()
        {
            return *this;
        }

        constexpr OIterSize &operator++()
        {
            return *this;
        }

        constexpr void operator++(int)
        {
        }

      private:
        uint_least64_t *_count;
    };

    template <typename UpstreamWriterT>
    struct InterceptFDWriter
    {
        UpstreamWriterT &W;
        std::pmr::vector<const void *> *fds;

        void write_fd(const void *fd)
        {
            if (!fds) {
                return;
            }
            fds->push_back(fd);
        }

        void operator()(const void *fd)
        {
            write_fd(fd);
        };

        void operator()(void *fd)
        {
            write_fd(fd);
        };

        template <typename T>
        void operator()(const T &v)
        {
            W(v);
        }
    };

    template <typename MessageT, typename UpstreamWriterT>
    static void write_args(const MessageT &M, UpstreamWriterT &W)
    {
        auto &metas = MessageT::Meta::template args_meta<Unix::WlTags>;
        auto F = [&](auto... meta) { ((W(M.*(std::get<1>(meta)))), ...); };
        std::apply(F, metas);
    }

    std::pmr::vector<std::byte> data;
    std::pmr::vector<const void *> fds;
};

} // namespace wlalat
