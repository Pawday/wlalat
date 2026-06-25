#pragma once

#include "ArgsIterator.hh"
#include "Binary.hh"
#include "Error.hh"
#include "Types.hh"
#include "Writer.hh"

#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <iterator>
#include <memory>
#include <memory_resource>
#include <span>
#include <variant>
#include <vector>

namespace wlalat
{

template <typename FDT>
struct MessageSerializer
{
    MessageSerializer() = default;
    MessageSerializer(std::pmr::memory_resource *res) : data{res}, fds{res}
    {
    }

    struct WriteOutputData
    {
        std::span<const std::byte> message;
        std::span<const FDT *> fds;
    };

    template <typename MessageT>
    WriteOutputData operator()(Object object_id, const MessageT &msg)
    {
        data.clear();
        fds.clear();
        auto oiter = std::back_inserter(data);

        uint_least64_t message_size = 0;
        OIterSize size_counter{message_size};
        MsgWriteVisitor size_v{size_counter, nullptr};
        std::visit(size_v, msg);
        message_size += 8;

        if (message_size > 0xffff) {
            throw Error::from_cstring("payload is to big");
        }

        auto msg_id_data = tole32(object_id.raw());
        std::ranges::copy(msg_id_data, oiter);

        uint32_t size_opcode_pair = 0;
        size_opcode_pair |= message_size;
        size_opcode_pair <<= 16;
        size_opcode_pair |= msg.opcode();
        auto size_opcode_pair_data = tole32(size_opcode_pair);
        std::ranges::copy(size_opcode_pair_data, oiter);

        MsgWriteVisitor V{oiter, std::addressof(fds)};
        std::visit(V, msg);

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
        std::pmr::vector<const FDT *> *fds;

        void operator()(const FDT &fd)
        {
            if (!fds) {
                return;
            }
            fds->push_back(&fd);
        };

        template <typename T>
        void operator()(const T &v)
        {
            W(v);
        }
    };

    template <typename OIterT>
    struct MsgWriteVisitor
    {
        OIterT oiter;
        std::pmr::vector<const FDT *> *fds;
        template <typename MessageT>
        void operator()(const MessageT &M)
        {
            Writer W{oiter};
            InterceptFDWriter fd_itercept_W{W, fds};
            ArgsIterator{fd_itercept_W, M};
        }
    };

    std::pmr::vector<std::byte> data;
    std::pmr::vector<const FDT *> fds;
};

} // namespace wlalat
