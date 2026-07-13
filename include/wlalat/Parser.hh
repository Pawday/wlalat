#pragma once

#include "ArrayParser.hh"
#include "Binary.hh"
#include "StringParser.hh"
#include "Types.hh"

#include <cstddef>
#include <cstdint>

#include <optional>
#include <span>
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
    bool has(std::type_identity<Object>)   const { return has(std::type_identity<Numeric>{}); }
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
        ArrayParser P{_data};
        if (!P.parse()) {
            return false;
        }
        return true;
    }

    uint_least32_t next(std::type_identity<Numeric>)
    {
        std::span<const std::byte, 4> d = _data.subspan<0, 4>();
        _data = _data.subspan(4);
        return fle32(d);
    }

    // clang-format off
    using Obj = Object;
    int_least32_t   next(std::type_identity<Int>)   { return  next(std::type_identity<Numeric>{}); };
    uint_least32_t  next(std::type_identity<UInt>)  { return  next(std::type_identity<Numeric>{}); };
    uint_least32_t  next(std::type_identity<Obj>)   { return  next(std::type_identity<Numeric>{}); };
    uint_least32_t  next(std::type_identity<Fixed>) { return  next(std::type_identity<Numeric>{}); };
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
        ArrayParser P{_data};
        return Array{P.parse().value()};
    }

    template <typename T, typename TagT>
    bool operator()(T &o, std::type_identity<TagT> tid)
    {
        if (!has(tid)) {
            return false;
        }
        o = next(tid);
        return true;
    }

  private:
    std::span<const std::byte> _data;
};

} // namespace wlalat
