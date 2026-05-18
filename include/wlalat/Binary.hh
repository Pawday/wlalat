#pragma once

#include <cstddef>
#include <cstdint>

#include <array>
#include <span>

namespace wlalat {

inline auto fle32(std::span<const std::byte, 4> D) -> uint32_t
{
    uint32_t o = 0;

    auto shifted_at = [&](size_t idx) {
        uint32_t v = std::to_integer<uint32_t>(D[idx]);
        v <<= (idx * 8);
        return v;
    };

    o |= shifted_at(0);
    o |= shifted_at(1);
    o |= shifted_at(2);
    o |= shifted_at(3);

    return o;
}

inline auto tole32(uint32_t N) -> std::array<std::byte, 4>
{
    auto byte_at = [N](size_t idx) {
        uint32_t mask = 0xff;
        mask <<= (idx * 8);
        uint32_t masked = N & mask;
        uint8_t o = masked >> (idx * 8);
        return std::byte{o};
    };

    std::array<std::byte, 4> O{};
    O[0] = byte_at(0);
    O[1] = byte_at(1);
    O[2] = byte_at(2);
    O[3] = byte_at(3);
    return O;
}

} // namespace wlalat
