#pragma once

#include "Types.hh"
#include "XML.hh"

#include <cstddef>

#include <algorithm>
#include <array>
#include <format>
#include <functional>
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

namespace CodeGen
{

struct ProtocolNode;
struct InterfaceNode;
struct RequestNode;
struct EventNode;
struct ArgNode;
struct EnumNode;
struct EntryNode;
struct DescriptionNode;
struct CopyrightNode;

template <typename RawTagT>
using MappingType = std::pair<std::string_view, MetadataEntry RawTagT::*>;

template <typename RawTagT>
struct TagTraits;

template <typename T>
struct Index
{
    constexpr Index(size_t index) : _index{index}
    {
    }

    constexpr T &get(std::span<T> C)
    {
        if (C.size() < _index) {
            throw std::out_of_range{"Index::at"};
        }
        return C[_index];
    }

    constexpr size_t index() const
    {
        return _index;
    }

  private:
    size_t _index;
};

struct ProtocolRawTag : ProtocolMetadata
{
    std::vector<Index<InterfaceNode>> interfaces;
};

template <>
struct TagTraits<ProtocolRawTag>
{
    using Type = ProtocolRawTag;
    static constexpr std::string_view tag_name = "protocol";

    static constexpr const MappingType<Type> mappings[]{
        {"name", &Type::name},
    };
};

struct InterfaceRawTag : InterfaceMetadata
{
    std::vector<Index<RequestNode>> requests;
    std::vector<Index<EventNode>> events;
    std::vector<Index<EnumNode>> enums;
};

template <>
struct TagTraits<InterfaceRawTag>
{
    using Type = InterfaceRawTag;
    static constexpr std::string_view tag_name = "interface";

    static constexpr const MappingType<Type> mappings[]{
        {"name", &Type::name},
        {"version", &Type::version},
        {"frozen", &Type::frozen},
    };
};

struct RequestRawTag : RequestMetadata
{
    std::vector<Index<ArgNode>> args;
};

template <>
struct TagTraits<RequestRawTag>
{
    using Type = RequestRawTag;
    static constexpr std::string_view tag_name = "request";

    static constexpr const MappingType<Type> mappings[]{
        {"name", &Type::name},
        {"type", &Type::type},
        {"since", &Type::since},
    };
};

struct EventRawTag : EventMetadata
{
    std::vector<Index<ArgNode>> args;
};

template <>
struct TagTraits<EventRawTag>
{
    using Type = EventRawTag;
    static constexpr std::string_view tag_name = "event";

    static constexpr const MappingType<Type> mappings[]{
        {"name", &Type::name},
        {"since", &Type::since},
        {"type", &Type::type},
        {"deprecated-since", &Type::deprecated_since},
    };
};

struct ArgRawTag : ArgumentMetadata
{
};

template <>
struct TagTraits<ArgRawTag>
{
    using Type = ArgRawTag;
    static constexpr std::string_view tag_name = "arg";

    static constexpr const MappingType<Type> mappings[]{
        {"name", &Type::name},
        {"type", &Type::type},
        {"interface", &Type::interface},
        {"summary", &Type::summary},
        {"allow-null", &Type::allow_null},
        {"enum", &Type::enum_name},
    };
};

struct EnumRawTag : EnumMetadata
{
    std::vector<Index<EntryNode>> entries;
};

template <>
struct TagTraits<EnumRawTag>
{
    using Type = EnumRawTag;
    static constexpr std::string_view tag_name = "enum";

    static constexpr const MappingType<Type> mappings[]{
        {"name", &Type::name},
        {"since", &Type::since},
        {"bitfield", &Type::bitfield},
    };
};

struct EntryRawTag : EnumEntryMetadata
{
};

template <>
struct TagTraits<EntryRawTag>
{
    using Type = EntryRawTag;
    static constexpr std::string_view tag_name = "entry";

    static constexpr const MappingType<Type> mappings[]{
        {"name", &Type::name},
        {"value", &Type::value},
        {"summary", &Type::summary},
        {"since", &Type::since},
        {"deprecated-since", &Type::deprecated_since},
    };
};

struct DescriptionRawTag
{
    MetadataEntry summary;
};

template <>
struct TagTraits<DescriptionRawTag>
{
    using Type = DescriptionRawTag;

    static constexpr std::string_view tag_name = "description";

    static constexpr const MappingType<Type> mappings[]{
        {"summary", &Type::summary},
    };
};

struct CopyrightRawTag
{
};

template <>
struct TagTraits<CopyrightRawTag>
{
    using Type = CopyrightRawTag;

    static constexpr std::string_view tag_name = "copyright";
    static constexpr const MappingType<Type> mappings[]{{}};
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
    static constexpr auto from_tag_name(std::string_view tag_name)
        -> std::optional<RawTagVariant>
    {
        std::optional<RawTagVariant> o;
        [&]<typename... Alts>(std::variant<Alts...> *) {
            [[maybe_unused]] bool found =
                ((TagTraits<Alts>::tag_name == tag_name ? (o.emplace(Alts{}), true)
                                             : false) ||
                 ...);
        }(static_cast<RawTagVariant *>(nullptr));
        return o;
    }
};

struct EntryNode : EntryRawTag
{
};

template <> struct TagTraits<EntryNode> : TagTraits<EntryRawTag>{};

struct EnumNode : EnumRawTag
{
};

template <> struct TagTraits<EnumNode> : TagTraits<EnumRawTag>{};

struct ArgNode : ArgRawTag
{
};

template <> struct TagTraits<ArgNode> : TagTraits<ArgRawTag>{};

struct EventNode : EventRawTag
{
};

template <> struct TagTraits<EventNode> : TagTraits<EventRawTag>{};

struct RequestNode : RequestRawTag
{
};

template <> struct TagTraits<RequestNode> : TagTraits<RequestRawTag>{};

struct InterfaceNode : InterfaceRawTag
{
};

template <> struct TagTraits<InterfaceNode> : TagTraits<InterfaceRawTag>{};

struct ProtocolNode : ProtocolRawTag
{
};

template <> struct TagTraits<ProtocolNode> : TagTraits<ProtocolRawTag>{};

struct DescriptionNode : DescriptionRawTag
{
};

template <> struct TagTraits<DescriptionNode> : TagTraits<DescriptionRawTag>{};

struct CopyrightNode : CopyrightRawTag
{
};

template <> struct TagTraits<CopyrightNode> : TagTraits<CopyrightRawTag>{};

// clang-format off
template<typename RawTagT> struct RawTagToNodeMap;

template<> struct RawTagToNodeMap<ProtocolRawTag>    : std::type_identity<ProtocolNode> {};
template<> struct RawTagToNodeMap<InterfaceRawTag>   : std::type_identity<InterfaceNode> {};
template<> struct RawTagToNodeMap<RequestRawTag>     : std::type_identity<RequestNode> {};
template<> struct RawTagToNodeMap<EventRawTag>       : std::type_identity<EventNode> {};
template<> struct RawTagToNodeMap<ArgRawTag>         : std::type_identity<ArgNode> {};
template<> struct RawTagToNodeMap<EnumRawTag>        : std::type_identity<EnumNode> {};
template<> struct RawTagToNodeMap<EntryRawTag>       : std::type_identity<EntryNode> {};
template<> struct RawTagToNodeMap<DescriptionRawTag> : std::type_identity<DescriptionNode> {};
template<> struct RawTagToNodeMap<CopyrightRawTag>   : std::type_identity<CopyrightNode> {};

template<typename RawTagT>
using RawTagToNodeMapT = typename RawTagToNodeMap<RawTagT>::type;
// clang-format on

struct Node : std::variant<
                  ProtocolNode,
                  InterfaceNode,
                  RequestNode,
                  EventNode,
                  ArgNode,
                  EnumNode,
                  EntryNode,
                  DescriptionNode,
                  CopyrightNode>
{
    constexpr std::string_view tag_name() const
    {
        auto vis = []<typename Alt>(const Alt &) { return TagTraits<Alt>::tag_name; };
        return std::visit(vis, *this);
    }

    static constexpr auto from_tag_name(std::string_view tag_name)
        -> std::optional<Node>
    {
        std::optional<Node> o;
        [&]<typename... Alts>(std::variant<Alts...> *) {
            [[maybe_unused]] bool found =
                ((TagTraits<Alts>::tag_name == tag_name ? (o.emplace(Alts{}), true)
                                             : false) ||
                 ...);
        }(static_cast<Node *>(nullptr));
        return o;
    }
};

struct ProtocolTreeView
{
    constexpr ProtocolTreeView(std::span<const Node> nodes) : _nodes{nodes}
    {
    }

    template <typename SinkT>
    constexpr void protos_iterate(SinkT &sink)
    {
        for (auto &node : _nodes) {
            auto *proto_p = std::get_if<ProtocolNode>(&node);
            if (proto_p) {
                sink(*proto_p);
            }
        }
    }

    constexpr std::vector<CodeGen::Protocol> collect()
    {
        std::vector<CodeGen::Protocol> O;

        std::optional<std::reference_wrapper<CodeGen::Protocol>> active_proto;
        std::optional<std::reference_wrapper<CodeGen::Interface>> active_iface;
        std::optional<std::reference_wrapper<CodeGen::Enum>> active_enum;

        auto node_from_typed_index = [&]<typename T>(Index<T> idx) {
            if (idx.index() >= _nodes.size()) {
                throw std::out_of_range{"node_from_typed_index"};
            }
            const Node &node = _nodes[idx.index()];
            return std::get<T>(node);
        };

        using ArgsIndexes = std::vector<Index<ArgNode>>;
        auto collect_args = [&](const ArgsIndexes &args) {
            std::vector<CodeGen::Argument> O;
            auto sink = [&](const ArgNode &arg_node) {
                O.push_back(Argument{arg_node});
            };
            for (auto &idx : args) {
                sink(node_from_typed_index(idx));
            }
            return O;
        };

        using EntryIndexes = std::vector<Index<EntryNode>>;
        auto collect_entries = [&](const EntryIndexes &entries) {
            std::vector<CodeGen::EnumEntry> O;
            auto sink = [&](const EntryNode &entry_node) {
                O.push_back(EnumEntry{entry_node});
            };
            for (auto &entry : entries) {
                sink(node_from_typed_index(entry));
            }
            return O;
        };

        auto event_sink = [&](const EventNode &event_node) {
            CodeGen::Event ev{event_node};
            ev.args = collect_args(event_node.args);
            CodeGen::Interface &target_iface = active_iface.value();
            target_iface.events.push_back(std::move(ev));
        };

        auto request_sink = [&](const RequestNode &request_node) {
            CodeGen::Request req{request_node};
            req.args = collect_args(request_node.args);
            CodeGen::Interface &target_iface = active_iface.value();
            target_iface.requests.push_back(std::move(req));
        };

        auto enum_sink = [&](const EnumNode &enum_node) {
            CodeGen::Enum enum_v{enum_node};
            enum_v.entries = collect_entries(enum_node.entries);
            active_enum = std::ref(enum_v);

            CodeGen::Interface &target_iface = active_iface.value();
            target_iface.enums.push_back(std::move(enum_v));
        };

        auto iface_sink = [&](const InterfaceNode &iface_node) {
            CodeGen::Interface iface{iface_node};
            active_iface = std::ref(iface);

            for (auto &ev_idx : iface_node.events) {
                event_sink(node_from_typed_index(ev_idx));
            }

            for (auto &req_idx : iface_node.requests) {
                request_sink(node_from_typed_index(req_idx));
            }

            for (auto &enum_idx : iface_node.enums) {
                enum_sink(node_from_typed_index(enum_idx));
            }

            CodeGen::Protocol &target_proto = active_proto.value();
            target_proto.interfaces.push_back(std::move(iface));
        };

        auto proto_sink = [&](const ProtocolNode &proto_node) {
            CodeGen::Protocol proto{proto_node};
            active_proto = std::ref(proto);
            for (auto &iface_idx : proto_node.interfaces) {
                iface_sink(node_from_typed_index(iface_idx));
            }
            O.push_back(std::move(proto));
            active_proto.reset();
        };
        protos_iterate(proto_sink);

        return O;
    }

  private:
    std::span<const Node> _nodes;
};

struct ProtocolTreeBuilder
{
    constexpr void push(const RawTagVariant &raw_tag)
    {
        tag_start(raw_tag);
    }

    constexpr void pop(const RawTagVariant &raw_tag)
    {
        tag_end(raw_tag);
    }

    constexpr std::vector<Node> build() const
    {
        return _tree;
    }

  private:
    template <typename RawTagT, typename NodeT = RawTagToNodeMapT<RawTagT>>
    constexpr void
        bind_chained(const RawTagT &tag, std::vector<Index<NodeT>> &dst)
    {
        size_t index = _tree.size();
        _tree.push_back(Node{NodeT{tag}});
        _active_tags.push_back(Index<Node>{index});
        dst.push_back(Index<NodeT>{index});
    }

    template <typename RawTagT>
    constexpr void t_bind_stack_only(const RawTagT &tag)
    {
        using NodeT = std::remove_const_t<RawTagToNodeMapT<RawTagT>>;
        _active_tags.reserve(_active_tags.size() + 1);
        _tree.reserve(_tree.size() + 1);
        _active_tags.push_back(Index<Node>{_tree.size()});
        NodeT node{tag};
        _tree.push_back(Node{node});
    }

    template <typename NodeT>
    constexpr void tag_end_delete_check_alt()
    {
        if (_active_tags.empty()) {
            throw std::runtime_error{"Premature tag_end"};
        }

        Node &node = _active_tags.back().get(std::span{_tree});

        if (!std::holds_alternative<NodeT>(node)) {
            auto msg = std::format(
                "Cannot end [{}] tag with [{}]",
                node.tag_name(),
                TagTraits<NodeT>::tag_name);
            throw std::runtime_error{std::move(msg)};
        }
        _active_tags.pop_back();
    }

    template <typename RawTagT, typename NodeT>
    constexpr void bind(const RawTagT &raw_tag, NodeT &)
    {
        if not consteval {
            auto msg = std::format(
                "[generic binder] Cannot bind [{}] to [{}]",
                TagTraits<RawTagT>::tag_name,
                TagTraits<NodeT>::tag_name);
            throw std::runtime_error{std::move(msg)};
        }
    }

    template <typename RawTagT>
    constexpr void bind(const RawTagT &raw_tag, ProtocolNode &)
    {
        auto msg =
            std::format("Cannot bind [{}] to protocol", TagTraits<RawTagT>::tag_name);
        throw std::runtime_error{std::move(msg)};
    }

    constexpr void bind(const CopyrightRawTag &raw_tag, ProtocolNode &)
    {
        t_bind_stack_only(raw_tag);
    }

    template <typename DestNodeT>
    constexpr void bind(const DescriptionRawTag &raw_tag, DestNodeT &)
    {
        t_bind_stack_only(raw_tag);
    }

    constexpr void bind(const DescriptionRawTag &raw_tag, ProtocolNode &)
    {
        t_bind_stack_only(raw_tag);
    }

    constexpr void bind(const InterfaceRawTag &raw_tag, ProtocolNode &proto)
    {
        bind_chained(raw_tag, proto.interfaces);
    }

    constexpr void bind(const EnumRawTag &raw_tag, InterfaceNode &iface)
    {
        bind_chained(raw_tag, iface.enums);
    }

    constexpr void bind(const EntryRawTag &raw_tag, EnumNode &enum_node)
    {
        bind_chained(raw_tag, enum_node.entries);
    }

    constexpr void bind(const ArgRawTag &raw_tag, RequestNode &req)
    {
        bind_chained(raw_tag, req.args);
    }

    constexpr void bind(const ArgRawTag &raw_tag, EventNode &ev)
    {
        bind_chained(raw_tag, ev.args);
    }

    constexpr void bind(const RequestRawTag &raw_tag, InterfaceNode &iface)
    {
        bind_chained(raw_tag, iface.requests);
    }

    constexpr void bind(const EventRawTag &raw_tag, InterfaceNode &iface)
    {
        bind_chained(raw_tag, iface.events);
    }

    friend struct tag_start_visitor;
    struct tag_start_visitor
    {
        ProtocolTreeBuilder &B;

        template <typename RawTagT>
        constexpr void operator()(const RawTagT &raw_tag)
        {
            auto &active_tags = B._active_tags;
            if (active_tags.empty()) {
                auto msg = std::format(
                    "<{}> must not be a top level tag",
                    TagTraits<RawTagT>::tag_name);
                throw std::runtime_error{std::move(msg)};
            }

            auto ensure_size_for = []<typename T>(std::vector<T> &vec) {
                if (vec.capacity() == vec.size()) {
                    vec.reserve(vec.size() * 2);
                }
            };

            ensure_size_for(active_tags);
            ensure_size_for(B._tree);

            Index active_tag = B.active_tag_index();
            auto visitor = [&]<typename NodeT>(NodeT &node) {
                B.bind(raw_tag, node);
            };

            Node &node = active_tag.get(std::span{B._tree});
            std::visit(visitor, node);
        }

        constexpr void operator()(const ProtocolRawTag &raw_proto)
        {
            if (!B._active_tags.empty()) {
                throw std::runtime_error{"<protocol> must be a top level tag"};
            }

            ProtocolNode node{raw_proto};
            Index<Node> index{B._tree.size()};
            B._tree.push_back(Node{node});
            B._active_tags.push_back(index);
        }
    };

    constexpr void tag_start(const RawTagVariant &raw_tag)
    {
        std::visit(tag_start_visitor{*this}, raw_tag);
    }

    constexpr void tag_end(const RawTagVariant &raw_tag)
    {
        auto visitor = [&]<typename RawTagT>(const RawTagT &) {
            tag_end_delete_check_alt<RawTagToNodeMapT<RawTagT>>();
        };
        std::visit(visitor, raw_tag);
    }

    constexpr Index<Node> active_tag_index()
    {
        if (_active_tags.empty()) {
            throw std::out_of_range{"unfinished_tag: empty stack"};
        }
        return _active_tags.back();
    }

  private:
    std::vector<Node> _tree;
    std::vector<Index<Node>> _active_tags;
};

struct ProtocolParser
{
    constexpr void send(char c)
    {
        _xml.send(c);
    }

    constexpr std::vector<CodeGen::Protocol> parse()
    {
        _xml.finalize();
        return ProtocolTreeView{tree.build()}.collect();
    }

  private:
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

        template <typename RawTagT>
        constexpr void bind(RawTagT &tag)
        {
            auto &mappings = TagTraits<RawTagT>::mappings;
            auto same_key = [this](auto &mapping) {
                return mapping.first == key;
            };
            auto mapping_it = std::ranges::find_if(mappings, same_key);
            if (mapping_it == std::end(mappings)) {
                auto msg = std::format(
                    "Tag [{}]: Unknown attribute [{}]=[{}]",
                    TagTraits<RawTagT>::tag_name,
                    key,
                    value);
                throw std::runtime_error{std::move(msg)};
            }
            MetadataEntry &val = tag.*(mapping_it->second);
            if (val) {
                auto msg = std::format(
                    "Tag [{}]: Duplicate attribute [{}]=[{}] (prev is [{}])",
                    TagTraits<RawTagT>::tag_name,
                    key,
                    value,
                    val.value());
                throw std::runtime_error{std::move(msg)};
            }
            val = value;
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

    [[nodiscard("Possible error message")]] constexpr std::optional<std::string>
        process_tag_string(std::string_view tag)
    {
        using TagParser = xml::TagParser;
        TagParser p{tag};

        auto tag_type_op = p.type();
        auto tag_name_op = p.name();
        if (!tag_name_op || !tag_type_op) {
            return std::format("Bad tag [{}]", tag);
        }

        auto tag_type = tag_type_op.value();
        std::string_view tag_name = tag_name_op.value();
        if (tag_name == "xml") {
            return {};
        }

        auto raw_tag_op = RawTagVariant::from_tag_name(tag_name);
        if (!raw_tag_op) {
            return std::format("Unknown tag [{}]", tag_name);
        }
        RawTagVariant raw_tag = raw_tag_op.value();
        TagVariantAttrBinder b{raw_tag};
        p.attribs(b);

        switch (tag_type) {
            case TagParser::UNPAIRED:
                tree.push(raw_tag);
                tree.pop(raw_tag);
                break;
            case TagParser::PAIR_START:
                tree.push(raw_tag);
                break;
            case TagParser::PAIR_END:
                tree.pop(raw_tag);
                break;
        }

        return {};
    }

    constexpr void on_data(std::string_view data)
    {
    }

    constexpr void on_comment(std::string_view data)
    {
    }

    struct CallbackObj
    {
        ProtocolParser *p;

        constexpr void on_tag(std::string_view tag)
        {
            auto err = p->process_tag_string(tag);
            if (err) {
                throw std::runtime_error{err.value()};
            }
        }

        constexpr void on_data(std::string_view data)
        {
            p->on_data(data);
        }

        constexpr void on_comment(std::string_view comment)
        {
            p->on_comment(comment);
        }
    };
    friend struct CallbackObj;

    CallbackObj _xml_callback{this};
    xml::Parser<CallbackObj> _xml{&_xml_callback};

    ProtocolTreeBuilder tree;
};

}; // namespace CodeGen
}; // namespace wlalat
