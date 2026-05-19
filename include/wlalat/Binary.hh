#pragma once

#include <cstddef>
#include <cstdint>

#include <array>
#include <span>

namespace wlalat {

constexpr auto fle32(std::span<const std::byte, 4> data) -> uint_fast32_t
{
    const std::byte *D = data.data();
    uint_fast32_t o = 0;
    o <<= 8; o |= std::to_integer<uint32_t>(D[3]);
    o <<= 8; o |= std::to_integer<uint32_t>(D[2]);
    o <<= 8; o |= std::to_integer<uint32_t>(D[1]);
    o <<= 8; o |= std::to_integer<uint32_t>(D[0]);
    o &= 0xffffffff;
    return o;
}

constexpr auto tole32(uint_fast32_t N) -> std::array<std::byte, 4>
{
    std::array<std::byte, 4> O{};
    std::byte *D = O.data();

    unsigned char b;
    uint_fast32_t sh;
    sh = N >> (0 * 8); b = sh & 0xff; D[0] = std::byte{b};
    sh = N >> (1 * 8); b = sh & 0xff; D[1] = std::byte{b};
    sh = N >> (2 * 8); b = sh & 0xff; D[2] = std::byte{b};
    sh = N >> (3 * 8); b = sh & 0xff; D[3] = std::byte{b};
    return O;
}

} // namespace wlalat
