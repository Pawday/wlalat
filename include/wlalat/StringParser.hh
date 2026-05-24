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
        std::span<const std::byte> string_data;
        std::span<const std::byte> undefined_padding;

        uint_least64_t message_size() const
        {
            return 4 + string_data.size() + undefined_padding.size();
        };

        auto string_data_zero_term() const
            -> std::optional<std::span<const std::byte>>
        {
            auto O = string_data;
            if (O.empty()) {
                return {};
            }

            if (O.back() != std::byte{0}) {
                return {};
            }

            return O;
        }

        auto string() const -> std::optional<String>
        {
            auto data_zterm_op = string_data_zero_term();
            if (!data_zterm_op) {
                return {};
            }
            auto data_zterm = data_zterm_op.value();
            data_zterm = data_zterm.subspan(0, data_zterm.size() - 1);

            const char *start =
                reinterpret_cast<const char *>(data_zterm.data());
            const char *end = start + data_zterm.size();
            return std::basic_string_view<char>{start, end};
        }
    };

    std::optional<Layout> parse() const
    {
        if (_data.size() < 4) {
            return {};
        }
        auto head = _data.subspan(4);
        uint_least32_t string_size_with_zeroterm = fle32(_data.subspan<0, 4>());

        if (head.size() < string_size_with_zeroterm) {
            return {};
        }
        Layout O{};
        O.string_data = head.subspan(0, string_size_with_zeroterm);
        head = head.subspan(string_size_with_zeroterm);

        uint_least8_t undefined_padding_size = 0;
        if (string_size_with_zeroterm % 4 != 0) {
            undefined_padding_size = 4 - (string_size_with_zeroterm % 4);
        }

        if (head.size() < undefined_padding_size) {
            return {};
        }
        O.undefined_padding = head.subspan(0, undefined_padding_size);
        return O;
    }

private:
    std::span<const std::byte> _data;
};
} // namespace wlalat
