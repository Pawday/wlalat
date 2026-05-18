#pragma once

#include <cstddef>
#include <cstdint>

#include <span>

namespace wlalat {

struct Message
{
    uint32_t object_id;
    uint16_t opcode;
    std::span<const std::byte> payload;
};

}; // namespace wlalat
