#pragma once

#include "Binary.hh"
#include "Error.hh"
#include "Types.hh"

#include <cstddef>

#include <algorithm>
#include <array>
#include <ranges>
#include <type_traits>

namespace wlalat
{

template <typename OIterT>
struct Writer
{
    Writer(OIterT oiter) : _oiter{oiter}
    {
    }

    constexpr void write_numeric(const wlalat::Numeric &N)
    {
        auto N_data = tole32(N.raw());
        std::ranges::copy(N_data, _oiter);
    }

#define NUMERIC_OVERLOAD(WL_TYPE)                                              \
    constexpr void write(const void *N_ptr, std::type_identity<WL_TYPE>)       \
    {                                                                          \
        write_numeric(*static_cast<const wlalat::Numeric *>(N_ptr));           \
    }
    NUMERIC_OVERLOAD(wlalat::NewID);
    NUMERIC_OVERLOAD(wlalat::Object);
    NUMERIC_OVERLOAD(wlalat::Int);
    NUMERIC_OVERLOAD(wlalat::UInt);
#undef NUMERIC_OVERLOAD

    constexpr void
        write(const void *S_ptr, std::type_identity<wlalat::String> id)
    {
        auto &S = *static_cast<const wlalat::String *>(S_ptr);
        write(S, id);
    }

    constexpr void
        write(const wlalat::String &S, std::type_identity<wlalat::String>)
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
        zterm_pad[1] = std::byte{0x2l};
        zterm_pad[2] = std::byte{0xa1};
        zterm_pad[3] = std::byte{0xad};
#endif
        std::ranges::copy(tail, zterm_pad.begin());
        zterm_pad[tsz] = std::byte{0};

        write_numeric(S.size() + 1);
        std::ranges::copy(lead, _oiter);
        std::ranges::copy(zterm_pad, _oiter);
    }

    template <typename TagT>
    constexpr void operator()(const void *v, TagT id)
    {
        write(v, id);
    }

  private:
    OIterT _oiter;
};

}; // namespace wlalat
