#pragma once

#include "Binary.hh"
#include "StringParser.hh"
#include "Types.hh"

#include <cstddef>

#include <optional>
#include <span>

namespace wlalat
{

struct Parser
{
    Parser(std::span<const std::byte> data) : _data{data}
    {
    }

    template <typename T>
    bool has() const = delete;
    // clang-format off
    template <> bool has<Numeric>()  const { return _data.size() >= 4; }
    template <> bool has<Int>()      const { return has<Numeric>(); }
    template <> bool has<UInt>()     const { return has<Numeric>(); }
    template <> bool has<Fixed>()    const { return has<Numeric>(); }
    // clang-format on

    template <>
    bool has<String>() const
    {
        StringParser p{_data};
        std::optional<StringParser::Layout> parsed_op = p.parse();
        return parsed_op.has_value();
    }

    template <typename T>
    T next() = delete;
    template <>
    Numeric next()
    {
        std::span<const std::byte, 4> d = _data.subspan<0, 4>();
        _data = _data.subspan(4);
        return Numeric{fle32(d)};
    }

    // clang-format off
    template <> Int   next() { return Int   { next<Numeric>()}; };
    template <> UInt  next() { return UInt  { next<Numeric>()}; };
    template <> Fixed next() { return Fixed { next<Numeric>()}; };
    // clang-format on

    template <>
    String next()
    {
        StringParser p{_data};
        StringParser::Layout parsed = p.parse().value();
        String O = parsed.string().value();
        _data = _data.subspan(parsed.message_size());
        return O;
    }

  private:
    std::span<const std::byte> _data;
};

} // namespace wlalat
