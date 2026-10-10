#pragma once

#include "Types.hh"

#include <cstddef>

#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace wlalat
{

namespace Protocol
{

template <typename OIterT>
struct Formatter
{
    constexpr Formatter(OIterT i, bool enable_indent = false) : _i{i}
    {
        if (enable_indent) {
            _current_indent.emplace(0);
        }
    }

    constexpr auto indent()
    {
        if (!_current_indent) {
            return _i;
        }
        return std::format_to(_i, "{: ^{}}", "", _current_indent.value());
    }

    constexpr auto push_indent()
    {
        if (!_current_indent) {
            return _i;
        }
        _current_indent.value()++;
        _i = std::format_to(_i, "\n");
        return indent();
    }

    constexpr auto pop_indent()
    {
        if (!_current_indent) {
            return _i;
        }
        _current_indent.value()--;
        _i = std::format_to(_i, "\n");
        return indent();
    }

    constexpr auto newline_and_indent()
    {
        if (!_current_indent) {
            return _i;
        }
        return std::format_to(_i, "\n{: ^{}}", "", _current_indent.value());
    }

    constexpr auto format(const Attribute &info)
    {
        _i = std::format_to(_i, "{{\"{}\",\"{}\"}}", info.key, info.value);
        return _i;
    }

    constexpr auto format(const Protocol &info)
    {
        _i = std::format_to(_i, "{{/*Protocol*/");
        _i = push_indent();
        _i = format_array(info.attributes, "Attribute");
        *(_i++) = ',';
        _i = newline_and_indent();
        _i = format_array(info.interfaces, "Interface");
        *(_i++) = ',';
        _i = newline_and_indent();
        _i = format(info.copyright);
        _i = pop_indent();
        _i = std::format_to(_i, "}}");
        return _i;
    }

    constexpr auto format(const Description &info)
    {
        _i = std::format_to(_i, "{{/*Description*/");
        _i = push_indent();
        _i = format_array(info.attributes, "Attribute");
        *(_i++) = ',';
        _i = newline_and_indent();
        _i = format_array(info.content_lines, "string");
        _i = pop_indent();
        _i = std::format_to(_i, "}}");
        return _i;
    }

    constexpr auto format(const Argument &info)
    {
        _i = std::format_to(_i, "{{/*Argument*/");
        _i = push_indent();
        _i = format_array(info.attributes, "Attribute");
        _i = pop_indent();
        _i = std::format_to(_i, "}}");
        return _i;
    }

    constexpr auto format(const Request &info)
    {
        _i = std::format_to(_i, "{{/*Request*/");
        _i = push_indent();
        _i = format_array(info.attributes, "Attribute");
        *(_i++) = ',';
        _i = newline_and_indent();
        _i = format_array(info.arguments, "Argument");
        *(_i++) = ',';
        _i = newline_and_indent();
        _i = format(info.description);
        _i = pop_indent();
        _i = std::format_to(_i, "}}");
        return _i;
    }

    constexpr auto format(const Event &info)
    {
        _i = std::format_to(_i, "{{/*Event*/");
        _i = push_indent();
        _i = format_array(info.attributes, "Attribute");
        *(_i++) = ',';
        _i = newline_and_indent();
        _i = format_array(info.arguments, "Argument");
        *(_i++) = ',';
        _i = newline_and_indent();
        _i = format(info.description);
        _i = pop_indent();
        _i = std::format_to(_i, "}}");
        return _i;
    }

    constexpr auto format(const EnumEntry &info)
    {
        _i = std::format_to(_i, "{{/*EnumEntry*/");
        _i = push_indent();
        _i = format_array(info.attributes, "Attribute");
        *(_i++) = ',';
        _i = newline_and_indent();
        _i = format(info.description);
        _i = pop_indent();
        _i = std::format_to(_i, "}}");
        return _i;
    }

    constexpr auto format(const Enum &info)
    {
        _i = std::format_to(_i, "{{/*Enum*/");
        _i = push_indent();
        _i = format_array(info.attributes, "Attribute");
        *(_i++) = ',';
        _i = newline_and_indent();
        _i = format_array(info.entries, "EnumEntry");
        *(_i++) = ',';
        _i = newline_and_indent();
        _i = format(info.description);
        _i = pop_indent();
        _i = std::format_to(_i, "}}");
        return _i;
    }

    constexpr auto format(const Interface &info)
    {
        _i = std::format_to(_i, "{{/*Interface*/");
        _i = push_indent();
        _i = format_array(info.attributes, "Attribute");
        *(_i++) = ',';
        _i = newline_and_indent();
        _i = format_array(info.requests, "Request");
        *(_i++) = ',';
        _i = newline_and_indent();
        _i = format_array(info.events, "Event");
        *(_i++) = ',';
        _i = newline_and_indent();
        _i = format_array(info.enums, "Enum");
        *(_i++) = ',';
        _i = newline_and_indent();
        _i = format(info.description);
        _i = pop_indent();
        _i = std::format_to(_i, "}}");
        return _i;
    }

    constexpr auto format(const Copyright &info)
    {
        _i = std::format_to(_i, "{{/*Copyright*/");
        _i = push_indent();
        _i = format_array(info.attributes, "Attribute");
        *(_i++) = ',';
        _i = newline_and_indent();
        _i = format_array(info.content_lines, "string");
        _i = pop_indent();
        _i = std::format_to(_i, "}}");
        return _i;
    }

    constexpr auto format(const std::string &str)
    {
        *(_i++) = '"';
        for (auto c : str) {
            switch (c) {
                case '\"':
                    *(_i++) = '\\';
                    *(_i++) = '\"';
                    continue;
                default:
                    *(_i++) = c;
            }
        }
        *(_i++) = '"';
        return _i;
    }

    template <typename T>
    constexpr auto
        format_array(const std::vector<T> &arr, std::string_view type)
    {
        bool f = true;
        _i = std::format_to(_i, "{{/*{}[]*/", type);
        if (!arr.empty()) {
            _i = push_indent();
        }
        for (auto &el : arr) {
            if (!f) {
                *(_i++) = ',';
                _i = newline_and_indent();
            }
            f = false;
            _i = format(el);
            if (f) {
                _i = newline_and_indent();
            }
        }
        if (!arr.empty()) {
            pop_indent();
        }
        _i = std::format_to(_i, "}}");
        return _i;
    }

  private:
    OIterT _i;
    std::optional<size_t> _current_indent;
};

} // namespace Protocol
} // namespace wlalat
