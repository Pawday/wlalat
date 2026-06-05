#pragma once

#include "Binary.hh"
#include "StringParser.hh"
#include "Types.hh"

#include <cstddef>

#include <optional>
#include <span>
#include <stdexcept>
#include <type_traits>

namespace wlalat
{

struct Parser
{
    Parser(std::span<const std::byte> data) : _data{data}
    {
    }

    // clang-format off
    bool has(std::type_identity<Numeric>)  const { return _data.size() >= 4; }
    bool has(std::type_identity<Int>)      const { return has(std::type_identity<Numeric>{}); }
    bool has(std::type_identity<UInt>)     const { return has(std::type_identity<Numeric>{}); }
    bool has(std::type_identity<Fixed>)    const { return has(std::type_identity<Numeric>{}); }
    // clang-format on

    bool has(std::type_identity<String>) const
    {
        StringParser p{_data};
        std::optional<StringParser::Layout> parsed_op = p.parse();
        if (!parsed_op) {
            return false;
        }
        return parsed_op.value().c_string_or_nullptr() != nullptr;
    }

    bool has(std::type_identity<Array>) const
    {
        // TODO: Figure out array structure
        return false;
    }

    Numeric next(std::type_identity<Numeric>)
    {
        std::span<const std::byte, 4> d = _data.subspan<0, 4>();
        _data = _data.subspan(4);
        return Numeric{fle32(d)};
    }

    // clang-format off
    Int   next(std::type_identity<Int>)   { return Int   { next(std::type_identity<Numeric>{})}; };
    UInt  next(std::type_identity<UInt>)  { return UInt  { next(std::type_identity<Numeric>{})}; };
    Fixed next(std::type_identity<Fixed>) { return Fixed { next(std::type_identity<Numeric>{})}; };
    // clang-format on

    String next(std::type_identity<String>)
    {
        StringParser p{_data};
        StringParser::Layout parsed = p.parse().value();
        String O = parsed.string().value();
        _data = _data.subspan(parsed.skip_size());
        return O;
    }

    Array next(std::type_identity<Array>)
    {
        throw std::runtime_error{"Array Parser::next() is not implemented"};
    }

  private:
    std::span<const std::byte> _data;
};

} // namespace wlalat
