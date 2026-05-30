#pragma once

#include "Binary.hh"
#include "Error.hh"
#include "Types.hh"

#include <cstddef>

#include <algorithm>
#include <array>
#include <ranges>

namespace wlalat
{

template <typename OIterT>
struct Writer
{
    Writer(OIterT oiter) : _oiter{oiter}
    {
    }

    constexpr void write(const wlalat::Numeric &N)
    {
        auto N_data = tole32(N.raw());
        std::ranges::copy(N_data, _oiter);
    }

    constexpr void write(const wlalat::String &S)
    {
        namespace V = std::ranges::views;
        auto mkb = [](char c) {
            return std::byte{static_cast<unsigned char>(c)};
        };

        if (S.size() > 0xffffffffu - sizeof('\0')) {
            throw Error::from_cstring("String is to big for wayland");
        }

        auto tsz = S.size() % 4;
        auto toff = S.size() - tsz;
        auto lead = S | V::take(toff) | V::transform(mkb);
        auto tail = S | V::drop(toff) | V::take(tsz) | V::transform(mkb);
        std::array<std::byte, 4> zterm_pad{};

#if 1
        zterm_pad[1] = std::byte{0xab};
        zterm_pad[2] = std::byte{0xcd};
        zterm_pad[3] = std::byte{0xef};
#endif
        std::ranges::copy(tail, zterm_pad.begin());
        zterm_pad[tsz] = std::byte{0};

        write(S.size() + 1);
        std::ranges::copy(lead, _oiter);
        std::ranges::copy(zterm_pad, _oiter);
    }

  private:
    OIterT _oiter;
};

}; // namespace wlalat
