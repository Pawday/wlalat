#pragma once

#include <algorithm>
#include <array>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace wlalat
{

namespace xml
{

struct CommentBuffer // wlalat::xml
{
    constexpr std::optional<char> exchange(char c)
    {
        bool full = true;
        for (std::optional<char> &buf_c : _buf) {
            if (buf_c.has_value()) {
                continue;
            }
            full = false;
            buf_c = c;
            break;
        }

        if (!full) {
            return {};
        }

        char O = _buf[0].value();
        _buf[0].value() = _buf[1].value();
        _buf[1].value() = _buf[2].value();
        _buf[2].value() = _buf[3].value();
        _buf[3].value() = c;
        return O;
    }

    constexpr void flush_to(std::string &O)
    {
        for (std::optional<char> &buf_c : _buf) {
            if (buf_c) {
                O += buf_c.value();
            }
            buf_c.reset();
        }
    }

    constexpr bool comment_start()
    {
        bool O = true;
        O = O && _buf[0] && _buf[0].value() == '<';
        O = O && _buf[1] && _buf[1].value() == '!';
        O = O && _buf[2] && _buf[2].value() == '-';
        O = O && _buf[3] && _buf[3].value() == '-';
        return O;
    }

    constexpr bool comment_end()
    {
        bool O = true;
        O = O && _buf[0] && _buf[0].value() == '-';
        O = O && _buf[1] && _buf[1].value() == '-';
        O = O && _buf[2] && _buf[2].value() == '>';
        return O;
    }

    std::array<std::optional<char>, 4> _buf{};
};

struct TagParser // wlalat::xml
{
    constexpr TagParser(std::string_view string) : _s{string}
    {
    }

    constexpr std::optional<std::string_view> name() const
    {
        auto s = _s;
        if (s.empty()) {
            return {};
        }
        s = s.substr(1);

        if (s.empty()) {
            return {};
        }

        if (s.front() == '>') {
            return {};
        }

        switch (s.front()) {
            case '?':
            case '/':
                s = s.substr(1);
        }

        auto name_end = std::ranges::find_if(s, white_or_name_end);
        if (name_end == std::end(s)) {
            return {};
        }

        auto name_len = std::distance(s.begin(), name_end);
        return s.substr(0, name_len);
    }

    enum Type
    {
        UNPAIRED,
        PAIR_START,
        PAIR_END,
    };

    constexpr std::optional<Type> type() const
    {
        auto s = _s;
        if (s.size() < 2) {
            return {};
        }

        if (s[1] == '/') {
            return Type::PAIR_END;
        }

        if (s[s.size() - 2] == '/') {
            return Type::UNPAIRED;
        }

        return Type::PAIR_START;
    }

    template <typename AttrSinkT>
    constexpr void attribs(AttrSinkT &sink) const
    {
        auto s = _s;
        auto name_end = std::ranges::find_if(s, white);
        if (name_end == std::end(s)) {
            return;
        }
        s = s.substr(std::distance(s.begin(), name_end));

        while (true) {
            auto white_end = std::ranges::find_if_not(s, white);
            if (white_end == std::end(s)) {
                return;
            }
            s = s.substr(std::distance(s.begin(), white_end));

            auto q0 = std::ranges::find(s, '"');
            if (q0 == std::end(s)) {
                return;
            }
            auto q1 = std::find(q0 + 1, s.end(), '"');
            if (q1 == std::end(s)) {
                return;
            }

            auto attr_end = q1 + 1;
            if (attr_end == std::end(s)) {
                return;
            }
            auto attr_len = std::distance(s.begin(), attr_end);

            std::string_view attr = s.substr(0, attr_len);
            sink(attr);
            s = s.substr(attr_len);
        }
    }

  private:
    static constexpr bool white(char c)
    {
        switch (c) {
            case ' ':
            case '\n':
            case '\r':
            case '\t':
                return true;
        }

        return false;
    }

    static constexpr bool white_or_name_end(char c)
    {
        switch (c) {
            case '/':
            case '>':
                return true;
        }

        return white(c);
    }

    std::string_view _s;
};

template <typename ParserCallbackT>
struct Parser // wlalat::xml
{
    constexpr Parser(ParserCallbackT *callback) : _callback{*callback}
    {
    }

    constexpr void send(char c)
    {
        if (!_active_comment) {
            if (!_cmnt_b.comment_start()) {
                priv_send(_cmnt_b.exchange(c));
                return;
            }
            _active_comment = std::string{};
            _cmnt_b.flush_to(_active_comment.value());
            add_to_comment(c);
            return;
        }

        if (_cmnt_b.comment_start()) {
            throw std::runtime_error{"Found nested comment in xml"};
        }

        if (!_cmnt_b.comment_end()) {
            add_to_comment(c);
            return;
        }

        _cmnt_b.flush_to(_active_comment.value());
        if (_active_data) {
            _callback.on_data(_active_data.value());
            _active_data.reset();
        }
        _callback.on_comment(_active_comment.value());
        _active_comment.reset();
        priv_send(_cmnt_b.exchange(c));
    }

    constexpr void add_to_comment(char c)
    {
        auto next = _cmnt_b.exchange(c);
        if (!next) {
            return;
        }
        _active_comment.value() += next.value();
    }

    constexpr void finalize()
    {
        if (_active_comment) {
            if (!_cmnt_b.comment_end()) {
                throw std::runtime_error{"finalize: Unfinished comment"};
            }
            _cmnt_b.flush_to(_active_comment.value());
            _callback.on_comment(_active_comment.value());
            _active_comment.reset();
            return;
        }

        if (_cmnt_b.comment_start()) {
            throw std::runtime_error{"finalize: Trailing comment start"};
        }

        for (auto &c : _cmnt_b._buf) {
            priv_send(c);
            c.reset();
        }

        if (_active_tag) {
            throw std::runtime_error{"finalize: Unfinished tag"};
        }

        if (_active_data) {
            _cmnt_b.flush_to(_active_data.value());
            _callback.on_data(_active_data.value());
            _active_data.reset();
        }
    }

  private:
    constexpr void priv_send(const std::optional<char> &c_op)
    {
        if (!c_op) {
            return;
        }
        auto c = c_op.value();

        if (!_active_tag && !_active_data) {
            std::string v;
            v += c;
            if (c == '<') {
                _active_tag = std::move(v);
            } else {
                _active_data = std::move(v);
            }
            return;
        }

        if (_active_tag) {
            if (c == '<') {
                throw std::runtime_error{"Nested tag begin"};
            }

            _active_tag.value() += c;
            if (c != '>') {
                return;
            }

            _callback.on_tag(_active_tag.value());
            _active_tag.reset();
            _active_data = std::string{};
            return;
        }

        if (c != '<') {
            _active_data.value() += c;
            return;
        }

        _callback.on_data(_active_data.value());
        _active_data.reset();
        _active_tag = std::string{};
        _active_tag.value() += c;
        return;
    }

    CommentBuffer _cmnt_b;

    std::optional<std::string> _active_tag;
    std::optional<std::string> _active_data;
    std::optional<std::string> _active_comment;

    ParserCallbackT &_callback;
};

} // namespace xml
}; // namespace wlalat
