#pragma once

#include "Binary.hh"
#include "Error.hh"
#include "Message.hh"

#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <limits>
#include <memory_resource>
#include <span>
#include <vector>

namespace wlalat {

struct MessageSerializer
{
    MessageSerializer() = default;
    MessageSerializer(std::pmr::memory_resource *res) : data{res}
    {
    }

    std::span<const std::byte> operator()(MessageView msg)
    {
        uint16_t message_size = msg.payload.size() + 8;
        if (message_size > std::numeric_limits<uint16_t>::max()) {
            throw Error::from_cstring("MessageRef::payload is to big");
        }

        if (data.size() < message_size) {
            data.resize(message_size);
        }

        std::span<std::byte> write_head{data};
        write_head = write_head.subspan(0, message_size);

        auto msg_id_data = tole32(msg.object_id.raw());
        std::ranges::copy(msg_id_data, write_head.begin());
        write_head = write_head.subspan(msg_id_data.size());

        uint32_t size_opcode_pair = 0;
        size_opcode_pair |= message_size;
        size_opcode_pair <<= 16;
        size_opcode_pair |= msg.opcode;
        auto size_opcode_pair_data = tole32(size_opcode_pair);
        std::ranges::copy(size_opcode_pair_data, write_head.begin());
        write_head = write_head.subspan(size_opcode_pair_data.size());

        std::ranges::copy(msg.payload, write_head.begin());
        write_head = write_head.subspan(msg.payload.size());

        std::span<const std::byte> data_span{data};
        return data_span.subspan(0, message_size);
    }

  private:
    std::pmr::vector<std::byte> data;
};

} // namespace wlalat
