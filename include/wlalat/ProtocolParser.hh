#pragma once

#include "CodeGenInfo.hh"
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

namespace ProtocolParsing
{

using AttrString = std::optional<std::string>;

template <typename RawTagT>
using MappingType = std::pair<std::string_view, AttrString RawTagT::*>;

struct ProtocolRawTag
{
    using RawTagT = ProtocolRawTag;
    static constexpr std::string_view tag_name = "protocol";

    AttrString name;

    static constexpr const MappingType<RawTagT> mappings[]{
        {"name", &RawTagT::name},
    };
};

struct InterfaceRawTag
{
    using RawTagT = InterfaceRawTag;
    static constexpr std::string_view tag_name = "interface";

    AttrString name;
    AttrString version;
    AttrString frozen;

    static constexpr const MappingType<RawTagT> mappings[]{
        {"name", &RawTagT::name},
        {"version", &RawTagT::version},
        {"frozen", &RawTagT::frozen},
    };
};

struct RequestRawTag
{
    using RawTagT = RequestRawTag;
    static constexpr std::string_view tag_name = "request";

    AttrString name;
    AttrString type;
    AttrString since;

    static constexpr const MappingType<RawTagT> mappings[]{
        {"name", &RawTagT::name},
        {"type", &RawTagT::type},
        {"since", &RawTagT::since},
    };
};

struct EventRawTag
{
    using RawTagT = EventRawTag;
    static constexpr std::string_view tag_name = "event";

    AttrString name;
    AttrString since;
    AttrString type;
    AttrString deprecated_since;

    static constexpr const MappingType<RawTagT> mappings[]{
        {"name", &RawTagT::name},
        {"since", &RawTagT::since},
        {"type", &RawTagT::type},
        {"deprecated-since", &RawTagT::deprecated_since},
    };
};

struct ArgRawTag
{
    using RawTagT = ArgRawTag;

    static constexpr std::string_view tag_name = "arg";
    AttrString name;
    AttrString type;
    AttrString interface;
    AttrString summary;
    AttrString allow_null;
    AttrString enum_name;

    static constexpr const MappingType<RawTagT> mappings[]{
        {"name", &RawTagT::name},
        {"type", &RawTagT::type},
        {"interface", &RawTagT::interface},
        {"summary", &RawTagT::summary},
        {"allow-null", &RawTagT::allow_null},
        {"enum", &RawTagT::enum_name},
    };
};

struct EnumRawTag
{
    using RawTagT = EnumRawTag;

    static constexpr std::string_view tag_name = "enum";
    AttrString name;
    AttrString since;
    AttrString bitfield;

    static constexpr const MappingType<RawTagT> mappings[]{
        {"name", &RawTagT::name},
        {"since", &RawTagT::since},
        {"bitfield", &RawTagT::bitfield},
    };
};

struct EntryRawTag
{
    using RawTagT = EntryRawTag;

    static constexpr std::string_view tag_name = "entry";
    AttrString name;
    AttrString value;
    AttrString summary;
    AttrString since;
    AttrString deprecated_since;

    static constexpr const MappingType<RawTagT> mappings[]{
        {"name", &RawTagT::name},
        {"value", &RawTagT::value},
        {"summary", &RawTagT::summary},
        {"since", &RawTagT::since},
        {"deprecated-since", &RawTagT::deprecated_since},
    };
};

struct DescriptionRawTag
{
    using RawTagT = DescriptionRawTag;

    static constexpr std::string_view tag_name = "description";
    AttrString summary;

    static constexpr const MappingType<RawTagT> mappings[]{
        {"summary", &RawTagT::summary},
    };
};

struct CopyrightRawTag
{
    using RawTagT = CopyrightRawTag;

    static constexpr std::string_view tag_name = "copyright";
    static constexpr const MappingType<RawTagT> mappings[]{{}};
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
    constexpr std::string_view tag_name() const
    {
        auto vis = []<typename Alt>(const Alt &) { return Alt::tag_name; };
        return std::visit(vis, *this);
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
        auto same_name_alternative_it = std::ranges::find_if(array, name_match);
        if (same_name_alternative_it == std::end(array)) {
            return {};
        }
        return *same_name_alternative_it;
    }
};

struct Index
{
    constexpr Index(size_t index) : _index{index}
    {
    }

    template <typename T>
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

template <typename T>
struct IndexChainNode : Index
{
    std::optional<Index> next;
};

struct EntryNode : IndexChainNode<EntryNode>, EntryRawTag
{
};

struct EnumNode : IndexChainNode<EnumNode>, EnumRawTag
{
    std::optional<IndexChainNode<EntryNode>> entries;
};

struct ArgNode : IndexChainNode<ArgNode>, ArgRawTag
{
};

struct EventNode : IndexChainNode<EventNode>, EventRawTag
{
    std::optional<IndexChainNode<ArgNode>> args;
};

struct RequestNode : IndexChainNode<RequestNode>, RequestRawTag
{
    std::optional<IndexChainNode<ArgNode>> args;
};

struct InterfaceNode : IndexChainNode<InterfaceNode>, InterfaceRawTag
{
    std::optional<IndexChainNode<RequestNode>> requests;
    std::optional<IndexChainNode<EventNode>> events;
    std::optional<IndexChainNode<EnumNode>> enums;
};

struct ProtocolNode : IndexChainNode<ProtocolNode>, ProtocolRawTag
{
    std::optional<IndexChainNode<InterfaceNode>> interfaces;
};

struct DescriptionNode : IndexChainNode<DescriptionNode>, DescriptionRawTag
{
};

struct CopyrightNode : IndexChainNode<CopyrightNode>, CopyrightRawTag
{
};

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
        auto vis = []<typename Alt>(const Alt &) { return Alt::tag_name; };
        return std::visit(vis, *this);
    }

    constexpr Index node_index() const
    {
        auto vis = []<typename Alt>(const Alt &alt) { return alt.index(); };
        return std::visit(vis, *this);
    }

    constexpr std::optional<Index> node_index_next() const
    {
        auto v = []<typename AltT>(const AltT &alt) { return alt.next; };
        return std::visit(v, *this);
    }
};

struct ProtocolTreeView
{
    constexpr ProtocolTreeView(std::span<const Node> nodes) : _nodes{nodes}
    {
    }

    template <typename SinkT>
    constexpr void chain_iterate(Index start, SinkT &sink)
    {
        Index curr = start;
        while (true) {
            const Node &curr_node = curr.get(_nodes);
            sink(curr_node);
            auto next = curr_node.node_index_next();
            if (!next) {
                break;
            }
            curr = next.value();
        }
    }

    template <typename SinkT>
    constexpr void protos_iterate(SinkT &sink)
    {
        for (auto &node : _nodes) {
            auto *proto_p =
                std::get_if<wlalat::ProtocolParsing::ProtocolNode>(&node);
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
        std::optional<std::reference_wrapper<CodeGen::Event>> active_event;
        std::optional<std::reference_wrapper<CodeGen::Request>> active_request;
        std::optional<std::reference_wrapper<CodeGen::Enum>> active_enum;

        auto collect_args = [&](IndexChainNode<ArgNode> args_chain) {
            std::vector<CodeGen::Argument> O;
            auto sink = [&](const ProtocolParsing::Node &arg_node_v) {
                auto &arg_node = std::get<ProtocolParsing::ArgNode>(arg_node_v);
                CodeGen::Argument arg{};
                arg.name = arg_node.name;
                arg.type = arg_node.type;
                arg.interface = arg_node.interface;
                O.push_back(std::move(arg));
            };
            chain_iterate(args_chain, sink);
            return O;
        };

        auto collect_entries = [&](IndexChainNode<EntryNode> entry_chain) {
            std::vector<CodeGen::EnumEntry> O;
            auto sink = [&](const ProtocolParsing::Node &entry_node_v) {
                auto &entry_node =
                    std::get<ProtocolParsing::EntryNode>(entry_node_v);
                CodeGen::EnumEntry entry{};
                entry.name = entry_node.name;
                entry.value = entry_node.value;
                O.push_back(std::move(entry));
            };
            chain_iterate(entry_chain, sink);
            return O;
        };

        auto event_sink = [&](const ProtocolParsing::Node &event_node_v) {
            auto &event_node =
                std::get<ProtocolParsing::EventNode>(event_node_v);
            CodeGen::Event ev{};
            ev.name = event_node.name;
            if (event_node.args) {
                ev.args = collect_args(event_node.args.value());
            }
            active_event = std::ref(ev);

            CodeGen::Interface &target_iface = active_iface.value();
            target_iface.events.push_back(std::move(ev));
        };

        auto request_sink = [&](const ProtocolParsing::Node &requst_node_v) {
            auto &request_node =
                std::get<ProtocolParsing::RequestNode>(requst_node_v);
            CodeGen::Request req{};
            req.name = request_node.name;
            if (request_node.args) {
                req.args = collect_args(request_node.args.value());
            }
            active_request = std::ref(req);

            CodeGen::Interface &target_iface = active_iface.value();
            target_iface.requests.push_back(std::move(req));
        };

        auto enum_sink = [&](const ProtocolParsing::Node &enum_node_v) {
            auto &enum_node = std::get<ProtocolParsing::EnumNode>(enum_node_v);
            CodeGen::Enum enum_v{};
            enum_v.name = enum_node.name;
            if (enum_node.entries) {
                enum_v.entries = collect_entries(enum_node.entries.value());
            }
            active_enum = std::ref(enum_v);

            CodeGen::Interface &target_iface = active_iface.value();
            target_iface.enums.push_back(std::move(enum_v));
        };

        auto iface_sink = [&](const ProtocolParsing::Node &iface_node_v) {
            auto &iface_node =
                std::get<ProtocolParsing::InterfaceNode>(iface_node_v);
            CodeGen::Interface iface{};
            iface.name = iface_node.name;
            active_iface = std::ref(iface);

            if (iface_node.events) {
                chain_iterate(iface_node.events.value(), event_sink);
            }

            if (iface_node.requests) {
                chain_iterate(iface_node.requests.value(), request_sink);
            }

            if (iface_node.enums) {
                chain_iterate(iface_node.enums.value(), enum_sink);
            }

            CodeGen::Protocol &target_proto = active_proto.value();
            target_proto.interfaces.push_back(std::move(iface));
        };

        auto proto_sink = [&](const ProtocolParsing::ProtocolNode &proto_node) {
            CodeGen::Protocol proto{};
            proto.name = proto_node.name;
            active_proto = std::ref(proto);
            if (proto_node.interfaces) {
                chain_iterate(proto_node.interfaces.value(), iface_sink);
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

struct ProtocolTree : std::vector<Node>
{
    ProtocolTreeView view() const
    {
        return ProtocolTreeView{*this};
    }
};

struct ProtocolTreeBuilder
{
    constexpr void push(RawTagVariant raw_tag)
    {
        tag_start(raw_tag);
    }

    constexpr void pop(RawTagVariant raw_tag)
    {
        tag_end(raw_tag);
    }

    constexpr ProtocolTree build() const
    {
        return _tree;
    }

  private:
    ProtocolTreeView view() const
    {
        return ProtocolTreeView{_tree};
    }

    template <typename NodeT>
    constexpr IndexChainNode<NodeT> chain_end(IndexChainNode<NodeT> node)
    {
        std::optional<Index> last;
        auto sink = [&last](const Node &n) {
            last = n.node_index();
            if (!std::holds_alternative<NodeT>(n)) {
                throw std::runtime_error{"Bad alternative in chain"};
            }
        };
        view().chain_iterate(node.index(), sink);
        return IndexChainNode<NodeT>{last.value()};
    }

    template <typename T>
    struct TypedNodeIndex
    {
        size_t index;

        constexpr T &get(std::span<Node> nodes)
        {
            if (index >= nodes.size()) {
                throw std::out_of_range{"TypepedNodeIndex::get"};
            }
            return std::get<T>(nodes[index]);
        }
    };

    template <typename RawTagT, typename DstNodeT>
    constexpr void bind_chained(
        RawTagT &tag,
        TypedNodeIndex<DstNodeT> &target_node_idx,
        std::optional<IndexChainNode<RawTagToNodeMapT<RawTagT>>> DstNodeT::*
            chain_dst_ptr)
    {
        using NodeT = RawTagToNodeMapT<RawTagT>;
        NodeT node{_tree.size()};
        static_cast<NodeT::RawTagT &>(node) = tag;

        _tree.push_back(Node{node});
        _active_tags.push_back(node);

        DstNodeT &target_node = target_node_idx.get(_tree);

        std::optional<IndexChainNode<NodeT>> &dst_chain =
            std::invoke(chain_dst_ptr, target_node);
        if (!dst_chain) {
            dst_chain = node;
            return;
        }

        auto end_idx = chain_end(dst_chain.value());
        Node &end = end_idx.get(std::span{_tree});
        NodeT &end_t = std::get<NodeT>(end);
        end_t.next = node;
    }

    template <typename NodeT>
    constexpr void t_bind_stack_only(NodeT::RawTagT &tag)
    {
        NodeT node{_tree.size()};
        static_cast<NodeT::RawTagT &>(node) = tag;
        _tree.push_back(Node{node});
        _active_tags.push_back(node);
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
                NodeT::tag_name);
            throw std::runtime_error{std::move(msg)};
        }
        _active_tags.pop_back();
    }

    template <typename RawTagT, typename NodeT>
    constexpr void bind(RawTagT &raw_tag, TypedNodeIndex<NodeT>)
    {
        if not consteval {
            auto msg = std::format(
                "[generic binder] Cannot bind [{}] to [{}]",
                RawTagT::tag_name,
                NodeT::tag_name);
            throw std::runtime_error{std::move(msg)};
        }
    }

    template <typename RawTagT>
    constexpr void bind(RawTagT &raw_tag, TypedNodeIndex<ProtocolNode>)
    {
        auto msg =
            std::format("Cannot bind [{}] to protocol", RawTagT::tag_name);
        throw std::runtime_error{std::move(msg)};
    }

    constexpr void bind(CopyrightRawTag &raw_tag, TypedNodeIndex<ProtocolNode>)
    {
        t_bind_stack_only<CopyrightNode>(raw_tag);
    }

    template <typename DestNodeT>
    constexpr void bind(DescriptionRawTag &raw_tag, TypedNodeIndex<DestNodeT>)
    {
        t_bind_stack_only<DescriptionNode>(raw_tag);
    }

    constexpr void
        bind(DescriptionRawTag &raw_tag, TypedNodeIndex<ProtocolNode>)
    {
        t_bind_stack_only<DescriptionNode>(raw_tag);
    }

    constexpr void
        bind(InterfaceRawTag &raw_tag, TypedNodeIndex<ProtocolNode> &proto_idx)
    {
        bind_chained(raw_tag, proto_idx, &ProtocolNode::interfaces);
    }

    constexpr void
        bind(EnumRawTag &raw_tag, TypedNodeIndex<InterfaceNode> &iface_idx)
    {
        bind_chained(raw_tag, iface_idx, &InterfaceNode::enums);
    }

    constexpr void
        bind(EntryRawTag &raw_tag, TypedNodeIndex<EnumNode> &enum_idx)
    {
        bind_chained(raw_tag, enum_idx, &EnumNode::entries);
    }

    constexpr void bind(ArgRawTag &raw_tag, TypedNodeIndex<RequestNode> &req)
    {
        bind_chained(raw_tag, req, &RequestNode::args);
    }

    constexpr void bind(ArgRawTag &raw_tag, TypedNodeIndex<EventNode> &ev)
    {
        bind_chained(raw_tag, ev, &EventNode::args);
    }

    constexpr void
        bind(RequestRawTag &raw_tag, TypedNodeIndex<InterfaceNode> &iface_idx)
    {
        bind_chained(raw_tag, iface_idx, &InterfaceNode::requests);
    }

    constexpr void
        bind(EventRawTag &raw_tag, TypedNodeIndex<InterfaceNode> &iface_idx)
    {
        bind_chained(raw_tag, iface_idx, &InterfaceNode::events);
    }

    friend struct tag_start_visitor;
    struct tag_start_visitor
    {
        ProtocolTreeBuilder &B;

        template <typename RawTagT>
        constexpr void operator()(RawTagT &raw_tag)
        {
            auto &active_tags = B._active_tags;
            if (active_tags.empty()) {
                auto msg = std::format(
                    "<{}> must not be a top level tag", RawTagT::tag_name);
                throw std::runtime_error{std::move(msg)};
            }

            Index active_tag = B.active_tag_index();
            auto visitor = [&]<typename NodeT>(NodeT &) {
                TypedNodeIndex<NodeT> node_idx{active_tag.index()};
                B.bind(raw_tag, node_idx);
            };

            Node &node = active_tag.get(std::span{B._tree});
            std::visit(visitor, node);
        }

        constexpr void operator()(ProtocolRawTag &raw_proto)
        {
            if (!B._active_tags.empty()) {
                throw std::runtime_error{"<protocol> must be a top level tag"};
            }

            ProtocolNode node{B._tree.size()};
            static_cast<ProtocolRawTag &>(node) = raw_proto;
            B._tree.push_back(Node{node});
            B._active_tags.push_back(node);
        }
    };

    constexpr void tag_start(RawTagVariant raw_tag)
    {
        std::visit(tag_start_visitor{*this}, raw_tag);
    }

    constexpr void tag_end(RawTagVariant raw_tag)
    {
        auto visitor = [&]<typename RawTagT>(RawTagT &) {
            tag_end_delete_check_alt<RawTagToNodeMapT<RawTagT>>();
        };
        std::visit(visitor, raw_tag);
    }

    constexpr Index active_tag_index()
    {
        if (_active_tags.empty()) {
            throw std::out_of_range{"unfinished_tag: empty stack"};
        }
        return _active_tags.back();
    }

  private:
    ProtocolTree _tree;
    std::vector<Index> _active_tags;
};

struct ProtocolParser
{
    constexpr ProtocolParser(std::string_view string) : _s{string}
    {
    }

    constexpr ProtocolTree parse()
    {
        for (auto c : _s) {
            _xml.send(c);
        }
        _xml.finalize();
        return tree.build();
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
            auto &mappings = RawTagT::mappings;
            auto same_key = [this](auto &mapping) {
                return mapping.first == key;
            };
            auto mapping_it = std::ranges::find_if(mappings, same_key);
            if (mapping_it == std::end(mappings)) {
                auto msg = std::format(
                    "Tag [{}]: Unknown attribute [{}]=[{}]",
                    RawTagT::tag_name,
                    key,
                    value);
                throw std::runtime_error{std::move(msg)};
            }
            AttrString &val = tag.*(mapping_it->second);
            if (val) {
                auto msg = std::format(
                    "Tag [{}]: Duplicate attribute [{}]=[{}] (prev is [{}])",
                    RawTagT::tag_name,
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

    std::string_view _s;
    ProtocolTreeBuilder tree;
};

}; // namespace ProtocolParsing
}; // namespace wlalat
