#pragma once

#include "Binary.hh"
#include "Message.hh"

#include <cstddef>
#include <cstdint>

#include <optional>
#include <span>

namespace wlalat {

struct MessageParser
{
    MessageParser(std::span<std::byte> data) : _data{data}
    {
    }

    std::optional<uint32_t> object_id() const
    {
        if (_data.size() < 4) {
            return {};
        }
        return fle32(_data.subspan<0, 4>());
    }

    std::optional<uint16_t> opcode() const
    {
        if (_data.size() < 8) {
            return {};
        }
        uint32_t size_opcode_pair = fle32(_data.subspan<4, 4>());
        return size_opcode_pair & 0x0000ffff;
    }

    std::optional<uint16_t> size() const
    {
        if (_data.size() < 8) {
            return {};
        }

        uint32_t size_opcode_pair = fle32(_data.subspan<4, 4>());
        uint32_t size = size_opcode_pair & 0xffff0000;
        size >>= 16;
        return size;
    }

    auto payload() const -> std::optional<std::span<const std::byte>>
    {
        auto size_op = size();
        if (!size_op) {
            return {};
        }

        uint32_t size = size_op.value();
        if (_data.size() < size) {
            return {};
        }

        if (size < 8) {
            return {};
        }

        uint32_t payload_size = size - 8;
        auto payload_span = _data.subspan(8, payload_size);
        return payload_span;
    }

    std::optional<Message> try_parse()
    {
        auto object_id_op = object_id();
        if (!object_id_op) {
            return {};
        }

        auto opcode_op = opcode();
        if (!opcode_op) {
            return {};
        }

        auto payload_op = payload();
        if (!payload_op) {
            return {};
        }

        Message O{};
        O.object_id = object_id_op.value();
        O.opcode = opcode_op.value();
        O.payload = payload_op.value();
        return O;
    }

  private:
    std::span<std::byte> _data;
};

} // namespace wlalat
