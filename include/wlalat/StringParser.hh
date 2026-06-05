#pragma once

#include "Binary.hh"
#include "Types.hh"

#include <cstddef>
#include <cstdint>

#include <optional>
#include <span>
#include <string_view>

namespace wlalat
{

struct StringParser
{
    StringParser(std::span<const std::byte> data) : _data{data}
    {
    }

    struct Layout
    {
        constexpr Layout(std::span<const std::byte> data, uint_least32_t strlen)
            : _data{data}, _strlen{strlen}
        {
        }

        const char *c_string_or_nullptr() const
        {
            if (_data.size() < _strlen + 1) {
                return nullptr;
            }
            if (_data[_strlen] != std::byte{0}) {
                return nullptr;
            }
            return reinterpret_cast<const char *>(_data.data());
        }

        auto string() const -> std::optional<String>
        {
            const char *cstr = c_string_or_nullptr();
            if (cstr == nullptr) {
                return {};
            }
            const char *start = cstr;
            const char *end = start + _strlen;
            return std::basic_string_view<char>{start, end};
        }

        std::span<const std::byte> data() const
        {
            return _data;
        }

        auto skip_size() const
        {
            return 4 + _data.size();
        }

      private:
        std::span<const std::byte> _data;
        uint_least32_t _strlen;
    };

    std::optional<Layout> parse() const
    {
        if (_data.size() < 4) {
            return {};
        }
        auto head = _data.subspan(4);
        uint_least32_t string_size_with_zeroterm = fle32(_data.subspan<0, 4>());
        if (string_size_with_zeroterm < 1) {
            return {};
        }
        uint_least32_t strlen = string_size_with_zeroterm - 1;
        uint_least32_t padded_size = string_size_with_zeroterm;
        if (string_size_with_zeroterm % 4 != 0) {
            padded_size += 4 - padded_size % 4;
        }
        if (head.size() < padded_size) {
            return {};
        }

        return Layout{head.subspan(0, padded_size), strlen};
    }

  private:
    std::span<const std::byte> _data;
};
} // namespace wlalat
