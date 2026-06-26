#pragma once

#include <cstdint>

namespace wlalat
{

struct MessageHeader
{
    uint_least32_t object_id;
    uint_least16_t opcode;
    uint_least16_t size;
};

}; // namespace wlalat
