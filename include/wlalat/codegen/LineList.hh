#pragma once

#include <algorithm>
#include <format>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace wlalat
{

namespace CodeGen
{

struct LineList : std::vector<std::string>
{
    void operator+=(const char *s)
    {
        push_back(s);
    }

    void operator+=(std::string_view v)
    {
        push_back(std::string{v});
    }

    void operator+=(std::string &&s)
    {
        push_back(std::move(s));
    }

    void operator+=(LineList &&o)
    {
        for (std::string &s : o) {
            push_back(std::move(s));
        }
    }

    void indent()
    {
        for (std::string &s : *this) {
            s = std::format("    {}", s);
        }
    }

    void comma_sep()
    {
        auto need_comma_lines =
            *this | std::views::reverse | std::views::drop(1);
        auto add_comma = [](std::string &line) { line.append(","); };
        std::ranges::for_each(need_comma_lines, add_comma);
    }
};

} // namespace CodeGen
} // namespace wlalat
