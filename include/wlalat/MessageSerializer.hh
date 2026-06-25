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
#include <limits>
#include <memory_resource>
#include <span>
#include <variant>
#include <vector>

namespace wlalat
{

struct MessageSerializer
{
    MessageSerializer() = default;
    MessageSerializer(std::pmr::memory_resource *res) : data{res}, payload{res}
    {
    }

    template <typename MessageT>
    std::span<const std::byte> operator()(Object object_id, const MessageT &msg)
    {
        payload.clear();
        MsgWriteVisitor V{payload};
        std::visit(V, msg);

        uint16_t message_size = payload.size() + 8;
        if (message_size > std::numeric_limits<uint16_t>::max()) {
            throw Error::from_cstring("payload is to big");
        }

        if (data.size() < message_size) {
            data.resize(message_size);
        }

        std::span<std::byte> write_head{data};
        write_head = write_head.subspan(0, message_size);

        auto msg_id_data = tole32(object_id.raw());
        std::ranges::copy(msg_id_data, write_head.begin());
        write_head = write_head.subspan(msg_id_data.size());

        uint32_t size_opcode_pair = 0;
        size_opcode_pair |= message_size;
        size_opcode_pair <<= 16;
        size_opcode_pair |= msg.opcode();
        auto size_opcode_pair_data = tole32(size_opcode_pair);
        std::ranges::copy(size_opcode_pair_data, write_head.begin());
        write_head = write_head.subspan(size_opcode_pair_data.size());

        std::ranges::copy(payload, write_head.begin());
        write_head = write_head.subspan(payload.size());

        std::span<const std::byte> data_span{data};
        return data_span.subspan(0, message_size);
    }

  private:
    template <typename UpstreamWriterT>
    struct IgnoreIntFDWriter
    {
        UpstreamWriterT &W;

        void operator()(int) {};

        template <typename T>
        void operator()(const T &v)
        {
            W(v);
        }
    };

    struct MsgWriteVisitor
    {
        std::pmr::vector<std::byte> &payload;
        template <typename MessageT>
        void operator()(const MessageT &M)
        {
            Writer W{std::back_inserter(payload)};
            IgnoreIntFDWriter no_fd_W{W};
            ArgsIterator{no_fd_W, M};
        }
    };

    std::pmr::vector<std::byte> payload;
    std::pmr::vector<std::byte> data;
};

} // namespace wlalat
