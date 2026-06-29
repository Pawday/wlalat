#pragma once

#include "Binary.hh"

#include <cstddef>
#include <cstdint>

#include <optional>
#include <span>

namespace wlalat
{

struct ArrayParser
{
    ArrayParser(std::span<const std::byte> data) : _data{data}
    {
    }

    std::optional<std::span<const std::byte>> parse() const
    {
        if (_data.size() < 4) {
            return {};
        }
        auto payload = _data.subspan(4);
        uint_least32_t payload_size = fle32(_data.subspan<0, 4>());
        uint_least32_t padded_size = payload_size;
        if (payload_size % 4 != 0) {
            padded_size += 4 - padded_size % 4;
        }
        if (payload.size() < padded_size) {
            return {};
        }

        return payload.subspan(0, payload_size);
    }

  private:
    std::span<const std::byte> _data;
};
} // namespace wlalat
