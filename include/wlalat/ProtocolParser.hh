#pragma once

#include <algorithm>
#include <iterator>
#include <optional>
#include <print>
#include <string_view>
#include <variant>

namespace wlalat
{

struct ProtocolParser
{
    ProtocolParser(std::string_view string) : _s{string}
    {
    }

    void test_process()
    {
        process();
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

    struct TagContentInterface
    {
        std::string_view name;
        std::string_view version;
        std::string_view frozen;
    };

    struct TagParser
    {
        TagParser(std::string_view string) : _s{string}
        {
        }

        std::optional<std::string_view> name() const
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

        std::optional<Type> type() const
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
        void attribs(AttrSinkT &sink) const
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
        std::string_view _s;
    };

    struct TestAttribPrinter
    {
        std::string_view m_name;
        std::string_view type_str;
        void operator()(std::string_view attr)
        {
            std::println("[{}][{}] [{}]", type_str, m_name, attr);
        }
    };

    void process_tag_string(std::string_view tag)
    {
        TagParser p{tag};

        auto m_name_op = p.name();
        std::string_view m_name{"__AA__"};
        if (m_name_op) {
            m_name = m_name_op.value();
        }

        std::string_view type_str{"?"};
        auto type_op = p.type();
        if (type_op) {
            switch (type_op.value()) {
            case TagParser::UNPAIRED:
                type_str = "UNPAIR";
                break;
            case TagParser::PAIR_START:
                type_str = "START ";
                break;
            case TagParser::PAIR_END:
                type_str = "END   ";
                break;
            }
        }

        auto type = p.type();
        if (type && type.value() == TagParser::Type::PAIR_END) {
            std::println("[{}][{}]", type_str, m_name);
        }

        TestAttribPrinter printer{m_name, type_str};
        p.attribs(printer);
    }

    void process()
    {
        if (_s.empty()) {
            return;
        }

        auto tag_start = _s.find('<');
        if (tag_start == _s.npos) {
            on_data(_s);
            _s = std::string_view{};
            return;
        }

        std::string_view data = _s.substr(0, tag_start);
        on_data(data);
        _s = _s.substr(tag_start);

        auto tag_end_pos = _s.find('>');
        if (tag_end_pos == _s.npos) {
            return;
        }

        std::string_view tag_string = _s.substr(0, tag_end_pos + 1);
        process_tag_string(tag_string);
        _s = _s.substr(tag_string.size());
    }

    void on_data(std::string_view data)
    {
    }

    std::string_view _s;
};

}; // namespace wlalat
