#pragma once

#include "wlalat/Types.hh"
#include <cstddef>
#include <cstdint>

#include <span>

namespace wlalat {

struct MessageView
{
    Object object_id;
    uint16_t opcode;
    std::span<const std::byte> payload;
};

}; // namespace wlalat
