#pragma once

#include <array>
#include <optional>
#include <stdexcept>
#include <string>
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
