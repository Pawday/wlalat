#pragma once

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
    using RawTagT = EntryRawTag;
};

struct EnumNode : IndexChainNode<EnumNode>, EnumRawTag
{
    using RawTagT = EnumRawTag;
    std::optional<IndexChainNode<EntryNode>> entries;
};

struct ArgNode : IndexChainNode<ArgNode>, ArgRawTag
{
    using RawTagT = ArgRawTag;
};

struct EventNode : IndexChainNode<EventNode>, EventRawTag
{
    using RawTagT = EventRawTag;
    std::optional<IndexChainNode<ArgNode>> args;
};

struct RequestNode : IndexChainNode<RequestNode>, RequestRawTag
{
    using RawTagT = RequestRawTag;
    std::optional<IndexChainNode<ArgNode>> args;
};

struct InterfaceNode : IndexChainNode<InterfaceNode>, InterfaceRawTag
{
    using RawTagT = InterfaceRawTag;
    std::optional<IndexChainNode<RequestNode>> requests;
    std::optional<IndexChainNode<EventNode>> events;
    std::optional<IndexChainNode<EnumNode>> enums;
};

struct ProtocolNode : IndexChainNode<ProtocolNode>, ProtocolRawTag
{
    using RawTagT = ProtocolRawTag;
    std::optional<IndexChainNode<InterfaceNode>> interfaces;
};

struct DescriptionNode : IndexChainNode<DescriptionNode>, DescriptionRawTag
{
    using RawTagT = DescriptionRawTag;
};

struct CopyrightNode : IndexChainNode<CopyrightNode>, CopyrightRawTag
{
    using RawTagT = CopyrightRawTag;
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
        while (!_s.empty()) {
            auto err = process();
            if (err) {
                throw std::runtime_error{err.value()};
            }
        }
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

    [[nodiscard("Possible error message")]] constexpr std::optional<std::string>
        process_tag_string(std::string_view tag)
    {
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

    [[nodiscard("Possible error message")]] constexpr std::optional<std::string>
        process()
    {
        if (_s.empty()) {
            return "End of string";
        }

        auto tag_start = _s.find('<');
        if (tag_start == _s.npos) {
            on_data(_s);
            _s = std::string_view{};
            return {};
        }

        std::string_view data = _s.substr(0, tag_start);
        on_data(data);
        _s = _s.substr(tag_start);

        auto tag_end_pos = _s.find('>');
        if (tag_end_pos == _s.npos) {
            return "Cannot find tag end";
        }

        std::string_view tag_string = _s.substr(0, tag_end_pos + 1);

        bool is_comment = false;
        if (tag_string.size() >= 4 && tag_string.substr(0, 4) == "<!--") {
            is_comment = true;
        }

        if (is_comment) {
            _s = _s.substr(tag_string.size());
            return {};
        }

        auto err = process_tag_string(tag_string);
        if (err) {
            return err;
        }
        _s = _s.substr(tag_string.size());
        return {};
    }

    constexpr void on_data(std::string_view data)
    {
    }

    std::string_view _s;
    ProtocolTreeBuilder tree;
};

}; // namespace ProtocolParsing
}; // namespace wlalat
