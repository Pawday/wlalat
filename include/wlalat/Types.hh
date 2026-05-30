#pragma once

#include <cstddef>
#include <cstdint>

#include <span>
#include <string_view>

namespace wlalat
{

struct Numeric
{
    Numeric() = default;
    Numeric(uint_fast32_t v) : _v{v}
    {
    }

    int_fast32_t raw() const
    {
        return _v;
    }

  protected:
    uint_fast32_t _v;
};

struct Int : Numeric
{
    operator int_least32_t() const
    {
        return _v & 0xffffffffu;
    }
};

struct UInt : Numeric
{
    operator uint_least32_t() const
    {
        return _v & 0xffffffffu;
    }
};

struct String : std::basic_string_view<char>
{
    String() = default;
    String(std::basic_string_view<char> s) : std::basic_string_view<char>{s}
    {
    }
};

struct Fixed : Numeric
{
    // clang-format off
    double f() { return to_floating<float>(); }
    double d() { return to_floating<double>(); }
    operator float()  { return f(); }
    operator double() { return d(); }
    // clang-format on

  private:
    template <typename F>
    F to_floating() const
    {
        uint_fast32_t up = _v;
        up >>= 8;
        up &= 0x00ffffff;
        uint_fast32_t down_u = _v;
        down_u &= 0xff;
        F down = down_u;
        down /= 255;
        return up + down;
    };
};

struct Array : std::span<const std::byte>
{
};

} // namespace wlalat
