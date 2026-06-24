#pragma once

#include "wlalat/Types.hh"
#include <cstddef>
#include <cstdint>

#include <span>

namespace wlalat {

struct [[deprecated("Removing MessageView")]] MessageView
{
    Object object_id;
    uint16_t opcode;
    std::span<const std::byte> payload;
};

}; // namespace wlalat
