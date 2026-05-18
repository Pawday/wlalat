#pragma once

#include <cstring>

#include <algorithm>
#include <exception>
#include <format>
#include <source_location>
#include <span>

namespace wlalat {

struct Error : std::exception
{
    static Error from_cstring(
        const char *msg,
        std::source_location l = std::source_location::current()) noexcept
    {
        Error O{};
        std::span<char> O_msg{O._msg};
        std::ranges::fill(O_msg, 0);
        O_msg = O_msg.subspan(0, O_msg.size() - 1);
        std::format_to_n(
            O_msg.begin(),
            O_msg.size(),
            "{}:{} - {}",
            l.file_name(),
            l.line(),
            msg);
        return O;
    }

    static Error from_errno(
        int errno_v,
        std::source_location l = std::source_location::current()) noexcept
    {
        Error O{};
        std::span<char> O_msg{O._msg};
        std::ranges::fill(O_msg, 0);
        O_msg = O_msg.subspan(0, O_msg.size() - 1);
        std::format_to_n(
            O_msg.begin(),
            O_msg.size(),
            "{}:{} - errno={} ({})",
            l.file_name(),
            l.line(),
            errno_v,
            strerror(errno_v));
        return O;
    }

    const char *what() const noexcept override
    {
        return _msg;
    }

  private:
    char _msg[1024];
};

} // namespace wlalat
