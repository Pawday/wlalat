#pragma once

#include <cstddef>

#include <algorithm>
#include <array>
#include <format>
#include <iterator>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace wlalat
{

struct ProtocolParser
{
    constexpr ProtocolParser(std::string_view string) : _s{string}
    {
    }

    constexpr void test_process()
    {
        process();
    }

    std::vector<std::string> test_tags() const
    {
        std::vector<std::string> o;
        for (auto &t : tags) {
            std::string str = std::format(
                "<{}> name_attr=[{}]",
                t.tag_name(),
                t.name_attr().value_or("<NULL>"));
            o.push_back(std::move(str));
        }
        return o;
    }

    constexpr size_t test_n_tags() const
    {
        return tags.size();
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

    using AttrString = std::optional<std::string_view>;

    struct ProtocolRawTag
    {
        static constexpr std::string_view tag_name = "protocol";
        AttrString name;
    };

    struct InterfaceRawTag
    {
        static constexpr std::string_view tag_name = "interface";
        AttrString name;
        AttrString version;
        AttrString frozen;
    };

    struct RequestRawTag
    {
        static constexpr std::string_view tag_name = "request";
        AttrString name;
        AttrString type;
        AttrString since;
    };

    struct EventRawTag
    {
        static constexpr std::string_view tag_name = "event";
        AttrString name;
        AttrString since;
        AttrString type;
        AttrString deprecated_since;
    };

    struct ArgRawTag
    {
        static constexpr std::string_view tag_name = "arg";
        AttrString name;
        AttrString type;
        AttrString interface;
        AttrString summary;
        AttrString allow_null;
        AttrString enum_name;
    };

    struct EnumRawTag
    {
        static constexpr std::string_view tag_name = "enum";
        AttrString name;
        AttrString since;
        AttrString bitfield;
    };

    struct EntryRawTag
    {
        static constexpr std::string_view tag_name = "entry";
        AttrString name;
        AttrString value;
        AttrString summary;
        AttrString since;
    };

    struct DescriptionRawTag
    {
        static constexpr std::string_view tag_name = "description";
        AttrString summary;
    };

    struct CopyrightRawTag
    {
        static constexpr std::string_view tag_name = "copyright";
    };

    struct RawTagVariant : std::variant<
                               ProtocolRawTag,
                               InterfaceRawTag,
                               RequestRawTag,
                               EventRawTag,
                               ArgRawTag,
                               EnumRawTag,
                               EntryRawTag,
                               DescriptionRawTag,
                               CopyrightRawTag>
    {
        struct tag_name_visitor
        {
            template <typename Alternative>
            constexpr std::string_view operator()(const Alternative &)
            {
                return Alternative::tag_name;
            }
        };
        constexpr std::string_view tag_name() const
        {
            return std::visit(tag_name_visitor{}, *this);
        }

        struct name_attr_visitor
        {
            template <typename Alternative>
            constexpr std::optional<std::string_view>
                operator()(const Alternative &a)
            {
                return a.name;
            }

            constexpr std::optional<std::string_view>
                operator()(const CopyrightRawTag &a)
            {
                return {};
            }

            constexpr std::optional<std::string_view>
                operator()(const DescriptionRawTag &a)
            {
                return {};
            }
        };

        constexpr std::optional<std::string_view> name_attr() const
        {
            return std::visit(name_attr_visitor{}, *this);
        }

        template <typename... Alternatives>
        static constexpr auto
            make_alternatives_array(const std::variant<Alternatives...> &arg)
        {
            return std::array<
                RawTagVariant,
                std::variant_size_v<std::remove_cvref_t<decltype(arg)>>>{
                RawTagVariant{Alternatives{}}...};
        }

        static constexpr auto from_tag_name(std::string_view tag_name)
            -> std::optional<RawTagVariant>
        {
            auto name_match = [tag_name](RawTagVariant &v) {
                return v.tag_name() == tag_name;
            };
            auto array = make_alternatives_array(RawTagVariant{});
            auto same_name_alternative_it =
                std::ranges::find_if(array, name_match);
            if (same_name_alternative_it == std::end(array)) {
                return {};
            }
            return *same_name_alternative_it;
        }
    };

    struct TagParser
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
        std::string_view _s;
    };

    struct BindAttrVisitor
    {
        static constexpr std::optional<BindAttrVisitor>
            try_constuct(std::string_view attr)
        {
            auto eq_pos = attr.find('=');
            if (eq_pos == attr.npos) {
                return {};
            }

            std::string_view key = attr.substr(0, eq_pos);
            std::string_view value = attr.substr(eq_pos);
            if (value.empty()) {
                return {};
            }
            value = value.substr(1);
            if (value.size() < 2) {
                return {};
            }
            value = value.substr(1, value.size() - 2);

            BindAttrVisitor o;
            o.key = key;
            o.value = value;
            return o;
        }

        using MappingType =
            std::pair<std::string_view, std::optional<std::string_view> *>;

        constexpr void
            try_bind(std::span<MappingType> mappings, std::string_view tag_name)
        {
            auto same_key = [this](auto &mapping) {
                return mapping.first == key;
            };
            auto mapping_it = std::ranges::find_if(mappings, same_key);
            if (mapping_it == std::end(mappings)) {
                auto msg = std::format(
                    "Tag [{}]: Unknown attribute [{}]=[{}]",
                    tag_name,
                    key,
                    value);
                throw std::runtime_error{std::move(msg)};
            }

            std::optional<std::string_view> &val = *mapping_it->second;
            if (val) {
                auto msg = std::format(
                    "Tag [{}]: Duplicate attribute [{}]=[{}] (prev is [{}])",
                    tag_name,
                    key,
                    value,
                    val.value());
                throw std::runtime_error{std::move(msg)};
            }
            val = value;
        }

        constexpr void bind(ProtocolRawTag &t)
        {
            MappingType mappings[]{
                {"name", &t.name},
            };
            try_bind(mappings, t.tag_name);
        }

        constexpr void bind(InterfaceRawTag &t)
        {
            MappingType mappings[]{
                {"name", &t.name},
                {"version", &t.version},
                {"frozen", &t.frozen},
            };
            try_bind(mappings, t.tag_name);
        }

        constexpr void bind(RequestRawTag &t)
        {
            MappingType mappings[]{
                {"name", &t.name},
                {"type", &t.type},
                {"since", &t.since},
            };
            try_bind(mappings, t.tag_name);
        }

        constexpr void bind(EventRawTag &t)
        {
            MappingType mappings[]{
                {"name", &t.name},
                {"since", &t.since},
                {"type", &t.type},
                {"deprecated-since", &t.deprecated_since},
            };
            try_bind(mappings, t.tag_name);
        }

        constexpr void bind(ArgRawTag &t)
        {
            MappingType mappings[]{
                {"name", &t.name},
                {"type", &t.type},
                {"interface", &t.interface},
                {"summary", &t.summary},
                {"allow-null", &t.allow_null},
                {"enum", &t.enum_name},
            };
            try_bind(mappings, t.tag_name);
        }

        constexpr void bind(EnumRawTag &t)
        {
            MappingType mappings[]{
                {"name", &t.name},
                {"since", &t.since},
                {"bitfield", &t.bitfield},
            };
            try_bind(mappings, t.tag_name);
        }

        constexpr void bind(EntryRawTag &t)
        {
            MappingType mappings[]{
                {"name", &t.name},
                {"value", &t.value},
                {"summary", &t.summary},
                {"since", &t.since},
            };
            try_bind(mappings, t.tag_name);
        }

        constexpr void bind(DescriptionRawTag &t)
        {
            MappingType mappings[]{
                {"summary", &t.summary},
            };
            try_bind(mappings, t.tag_name);
        }

        constexpr void bind(CopyrightRawTag &t)
        {
        }

        template <typename T>
        constexpr void operator()(T &alt)
        {
            bind(alt);
        }

      private:
        BindAttrVisitor() = default;
        std::string_view key;
        std::string_view value;
    };

    struct TagVariantAttrBinder
    {
        RawTagVariant &tag_content;
        constexpr void operator()(std::string_view attr)
        {
            auto v = BindAttrVisitor::try_constuct(attr);
            if (!v) {
                return;
            }
            std::visit(v.value(), tag_content);
        }
    };

    constexpr void process_tag_string(std::string_view tag)
    {
        TagParser p{tag};

        auto tag_type_op = p.type();
        auto tag_name_op = p.name();
        if (!tag_name_op || !tag_type_op) {
            auto msg = std::format("Bad tag [{}]", tag);
            throw std::runtime_error{std::move(msg)};
        }

        auto tag_type = tag_type_op.value();
        std::string_view tag_name = tag_name_op.value();
        if (tag_name == "xml") {
            return;
        }

        auto tag_content_op = RawTagVariant::from_tag_name(tag_name);
        if (!tag_content_op) {
            auto msg = std::format("Unknown tag [{}]", tag_name);
            throw std::runtime_error{std::move(msg)};
        }
        RawTagVariant tag_content = tag_content_op.value();
        TagVariantAttrBinder b{tag_content};
        p.attribs(b);

        tags.push_back(tag_content);
    }
    std::vector<RawTagVariant> tags;

    constexpr void process()
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

        bool is_comment = false;
        if (tag_string.size() >= 4 && tag_string.substr(0, 4) == "<!--") {
            is_comment = true;
        }

        if (!is_comment) {
            process_tag_string(tag_string);
        }
        _s = _s.substr(tag_string.size());
    }

    constexpr void on_data(std::string_view data)
    {
    }

    std::string_view _s;
};

}; // namespace wlalat
