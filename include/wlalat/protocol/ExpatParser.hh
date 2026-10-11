#pragma once

#include "Types.hh"

#include <expat.h>
#include <expat_external.h>

#include <cstddef>

#include <format>
#include <functional>
#include <optional>
#include <ranges>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace wlalat
{

namespace Protocol
{

struct ExpatParser
{
    ExpatParser()
    {
        _p = XML_ParserCreate(nullptr);
        if (!_p) {
            release();
            throw std::runtime_error{"Cannot init expat"};
        }
        XML_SetUserData(_p, this);
        XML_SetElementHandler(_p, tag_start_st, tag_end_st);
        XML_SetCharacterDataHandler(_p, on_data_st);
        XML_SetCdataSectionHandler(_p, cdata_start, cdata_end);
    }

    void send(char c)
    {
        send({&c, 1});
    }

    void send(std::string_view data)
    {
        if (_proto.has_value()) {
            abort_parser("Done protocol");
        }

        XML_Status status = XML_Parse(_p, data.data(), data.size(), false);
        if (XML_STATUS_OK != status) {
            throw std::runtime_error{format_parse_failure_message(status)};
        }
    }

    void finalize()
    {
        auto status = XML_ParseBuffer(_p, 0, 1);
        if (XML_STATUS_OK != status) {
            throw std::runtime_error{format_parse_failure_message(status)};
        }

        if (!_proto.has_value()) {
            throw std::runtime_error{"No protocol parsed"};
        }
    }

    std::optional<Protocol> &proto()
    {
        return _proto;
    }

    ~ExpatParser()
    {
        release();
    }

    ExpatParser(const ExpatParser &) = delete;
    ExpatParser(ExpatParser &&) = delete;
    ExpatParser &operator=(const ExpatParser &) = delete;
    ExpatParser &operator=(ExpatParser &&) = delete;

  private:
    // clang-format off
    using TagVariant = std::variant
    <
        Protocol,
        Interface,
        Request,
        Event,
        Enum,
        EnumEntry,
        Argument,
        Description,
        Copyright
    >;
    // clang-format on

    struct Tag : TagVariant
    {
        using IDVariant =
            decltype([]<typename... Alts>(
                         std::type_identity<std::variant<Alts...>>) {
                return std::variant<std::type_identity<Alts>...>{};
            }(std::type_identity<TagVariant>{}));

        struct ID : IDVariant
        {
            ID(IDVariant v) : IDVariant{v}
            {
            }

            template <typename Alt>
            struct Naming
            {
                std::string_view name;
            };

            static auto namings()
            {
                return std::make_tuple(
                    Naming<Protocol>{"protocol"},
                    Naming<Interface>{"interface"},
                    Naming<Request>{"request"},
                    Naming<Event>{"event"},
                    Naming<Enum>{"enum"},
                    Naming<EnumEntry>{"entry"},
                    Naming<Argument>{"arg"},
                    Naming<Description>{"description"},
                    Naming<Copyright>{"copyright"});
            }

            template <typename Alt>
            static void create_if_match(
                const Naming<Alt> &naming,
                std::string_view name,
                std::optional<ID> &O)
            {
                if (O || name != naming.name) {
                    return;
                }
                O.emplace(std::type_identity<Alt>{});
            }

            static std::optional<ID> from_name(std::string_view name)
            {
                std::optional<ID> O;
                auto matcher = [&](auto... tag_namings) {
                    (..., create_if_match(tag_namings, name, O));
                };
                std::apply(matcher, namings());
                return O;
            }

            std::string_view name() const
            {
                return std::visit(
                    []<typename Alt>(std::type_identity<Alt>) {
                        return std::get<Naming<Alt>>(namings()).name;
                    },
                    *this);
            }
        };

        Tag(ID id)
            : TagVariant{std::visit(
                  []<typename Alt>(std::type_identity<Alt>) {
                      return TagVariant{Alt{}};
                  },
                  id)}
        {
        }

        ID id() const
        {
            return std::visit(
                []<typename Alt>(const Alt &) {
                    return ID{std::type_identity<Alt>{}};
                },
                *this);
        }

        bool is(ID id) const
        {
            return std::visit(
                [&]<typename Alt>(std::type_identity<Alt>) {
                    return std::holds_alternative<Alt>(*this);
                },
                id);
        }

        std::vector<Attribute> &attributes()
        {
            return std::visit(
                [&]<typename InfoT>(InfoT &info) {
                    return std::ref(info.attributes);
                },
                *this);
        }
    };

    struct BindVisitor
    {
        bool operator()(auto &bind_point, auto &top)
        {
            _bind_ok = true;
            bind(bind_point, top);
            return _bind_ok;
        }

      private:
        bool _bind_ok = true;

        template <typename BindPointAlt, typename TopAltT>
        void bind(BindPointAlt &, TopAltT &)
        {
            _bind_ok = false;
        }

        void bind(Interface &bind_point, Description &top)
        {
            bind_point.description = std::move(top);
        }

        void bind(Request &bind_point, Description &top)
        {
            bind_point.description = std::move(top);
        }

        void bind(Event &bind_point, Description &top)
        {
            bind_point.description = std::move(top);
        }

        void bind(Enum &bind_point, Description &top)
        {
            bind_point.description = std::move(top);
        }

        void bind(EnumEntry &bind_point, Description &top)
        {
            bind_point.description = std::move(top);
        }

        void bind(Request &bind_point, Argument &top)
        {
            bind_point.arguments.push_back(std::move(top));
        }

        void bind(Event &bind_point, Argument &top)
        {
            bind_point.arguments.push_back(std::move(top));
        }

        void bind(Interface &bind_point, Request &top)
        {
            bind_point.requests.push_back(std::move(top));
        }

        void bind(Interface &bind_point, Event &top)
        {
            bind_point.events.push_back(std::move(top));
        }

        void bind(Interface &bind_point, Enum &top)
        {
            bind_point.enums.push_back(std::move(top));
        }

        void bind(Enum &bind_point, EnumEntry &top)
        {
            bind_point.entries.push_back(std::move(top));
        }

        void bind(Protocol &bind_point, Interface &top)
        {
            bind_point.interfaces.push_back(std::move(top));
        }

        void bind(Protocol &bind_point, Copyright &top)
        {
            bind_point.copyright = std::move(top);
        }
    };

    struct DataBindVisitor
    {
        DataBindVisitor(std::string_view data) : data(data)
        {
        }

        template <typename T>
        void operator()(T &alt)
        {
            bind(alt);
        }

      private:
        std::string_view data;

        template <typename T>
        void bind(T &alt)
        {
            // Ignore irrelevant XML formatting
        }

        void bind(Description &desc)
        {
            collect_to(desc.content_lines);
        }

        void bind(Copyright &copycaca)
        {
            collect_to(copycaca.content_lines);
        }

        void collect_to(std::vector<std::string> &content_lines)
        {
            std::string_view delim{"\n"};
            auto lines = std::views::split(data, delim);
            for (auto line : lines) {
                content_lines.emplace_back(line.begin(), line.end());
            }
        }
    };

    std::optional<std::string> _abort_reason;
    void abort_parser(std::string abort_reason)
    {
        _abort_reason = std::move(abort_reason);
        XML_StopParser(_p, false);
    }

    std::string format_parse_failure_message(XML_Status status)
    {
        auto code = XML_GetErrorCode(_p);
        std::string abort_string;
        if (code == XML_ERROR_ABORTED) {
            abort_string = std::format(
                " abort_reason=[{}]", _abort_reason.value_or("not specified"));
        }

        return std::format(
            "Parse failure status=[{}] code=[{} ({})]{}",
            (int)status,
            (int)code,
            XML_ErrorString(code),
            abort_string);
    }

    void release()
    {
        if (_p) {
            XML_ParserFree(_p);
            _p = nullptr;
        }
    }

    static void XMLCALL tag_start_st(
        void *userData, const XML_Char *name, const XML_Char **atts)
    {
        static_cast<ExpatParser *>(userData)->tag_start(name, atts);
    }
    void tag_start(const XML_Char *name, const XML_Char **atts)
    {
        _active_data.reset();

        auto id_op = Tag::ID::from_name(name);
        if (!id_op) {
            abort_parser(std::format("Unknown tag [{}]", name));
            return;
        }

        _tag_stack.push(Tag{id_op.value()});

        auto &tag = _tag_stack.top();
        auto &attrs = tag.attributes();

        std::optional<std::string_view> k;
        std::optional<std::string_view> v;

        auto next = atts;
        while (*next != nullptr) {
            auto cur = next;
            next++;

            if (!k) {
                k = *cur;
                continue;
            }
            v = *cur;

            Attribute attr;
            attr.key = k.value();
            attr.value = v.value();
            attrs.push_back(std::move(attr));

            k.reset();
            v.reset();
        }
    }

    static void XMLCALL tag_end_st(void *userData, const XML_Char *name)
    {
        static_cast<ExpatParser *>(userData)->tag_end(name);
    }
    void tag_end(const XML_Char *name)
    {
        auto id_op = Tag::ID::from_name(name);
        if (!id_op) {
            abort_parser(std::format("Unknown tag [{}]", name));
            return;
        }
        Tag::ID &end_id = id_op.value();

        if (_tag_stack.empty()) {
            abort_parser("Empty stack on tag_end()");
            return;
        }

        Tag top = std::move(_tag_stack.top());
        _tag_stack.pop();

        if (!top.is(end_id)) {
            abort_parser(
                std::format(
                    "Cannot close tag [{}] with [{}]",
                    top.id().name(),
                    end_id.name()));
            return;
        }

        if (_active_data) {
            DataBindVisitor v{_active_data.value()};
            std::visit(v, top);
            _active_data.reset();
        }

        if (_tag_stack.empty()) {
            Protocol &proto = std::get<Protocol>(top);
            _proto.emplace(std::move(proto));
            return;
        }

        Tag &bind_point = _tag_stack.top();
        BindVisitor v{};
        bool bind_ok = std::visit(v, bind_point, top);
        if (!bind_ok) {
            auto msg = std::format(
                "Cannot bind [{}] to [{}]",
                top.id().name(),
                bind_point.id().name());
            abort_parser(msg);
        }
    }

    bool _is_cdata = false;
    std::optional<std::string> _active_data;

    static void XMLCALL cdata_start(void *userData)
    {
        static_cast<ExpatParser *>(userData)->_is_cdata = true;
    }
    static void XMLCALL cdata_end(void *userData)
    {
        static_cast<ExpatParser *>(userData)->_is_cdata = false;
    }

    static void XMLCALL on_data_st(void *userData, const XML_Char *s, int len)
    {
        static_cast<ExpatParser *>(userData)->on_data(s, len);
    }
    void on_data(const XML_Char *s, int len)
    {
        if (len < 0) [[unlikely]] {
            abort_parser("I found a bug in libxpat");
            return;
        }

        if (_is_cdata) {
            abort_parser("!!!!!!!!!!!!!CDATA!!!!!!!!!!!!!");
            return;
        }

        if (!_active_data) {
            _active_data.emplace();
        }
        std::string &active_data = _active_data.value();
        active_data.append(s, len);
    }

    XML_Parser _p = nullptr;

    std::stack<Tag> _tag_stack;
    std::optional<Protocol> _proto;
};

} // namespace Protocol
} // namespace wlalat
