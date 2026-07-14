#pragma once

#include "ProtocolParser.hh"

#include <array>
#include <cstddef>

#include <algorithm>
#include <format>
#include <functional>
#include <list>
#include <optional>
#include <ranges>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
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
};

static void comma_sep(LineList &lines)
{
    auto need_comma_lines = lines | std::views::reverse | std::views::drop(1);
    auto add_comma = [](std::string &line) { line.append(","); };
    std::ranges::for_each(need_comma_lines, add_comma);
}

// https://gitlab.freedesktop.org/wayland/wayland/-/commit/85a6a470873357089ffb968a176d5074fddd1756
struct AmogusArg : ProtocolParsing::ArgRawTag
{
    AmogusArg(ProtocolParsing::ArgRawTag raw) : ProtocolParsing::ArgRawTag{raw}
    {
        if (name) {
            _name = name.value();
            name = _name;
        }

        if (type) {
            _type = type.value();
            type = _type;
        }
    }
    std::string _name;
    std::string _type;

    static std::list<AmogusArg> collect_amogusified(
        ProtocolParsing::IndexChainNode<ProtocolParsing::ArgNode> args_start,
        ProtocolParsing::ProtocolTreeView view)
    {
        std::list<AmogusArg> O;
        auto my_sink = [&](const ProtocolParsing::Node &node) {
            auto &arg = std::get<ProtocolParsing::ArgNode>(node);
            auto name = arg.name.value();
            auto type = arg.type.value();

            bool is_amogus_new_id =
                type == "new_id" && !arg.interface.has_value();
            if (is_amogus_new_id) {

                ProtocolParsing::ArgRawTag am_tag{};
                std::string am_name;
                am_name = std::format("{}_interface_name_amogus_arg", name);
                am_tag.name = am_name;
                am_tag.type = "string";
                O.emplace_back(am_tag);

                am_name = std::format("{}_interface_version_amogus_arg", name);
                am_tag.name = am_name;
                am_tag.type = "uint";
                O.emplace_back(am_tag);
            }
            O.emplace_back(arg);
        };
        view.chain_iterate(args_start, my_sink);
        return O;
    }

    AmogusArg(const AmogusArg &) = delete;
    AmogusArg(AmogusArg &&) = delete;
    AmogusArg &operator=(const AmogusArg &) = delete;
    AmogusArg &operator=(AmogusArg &&) = delete;
};

struct Generator
{
    Generator(ProtocolParsing::ProtocolTreeView view) : _view{view}
    {
    }

    LineList generate()
    {
        LineList O;

        O += "#pragma once";
        O += "";
        O += "#include <wlalat/Traits.hh>";
        O += "";
        O += "#include <cstddef>";
        O += "#include <cstdint>";
        O += "";
        O += "#include <string_view>";
        O += "#include <tuple>";
        O += "#include <span>";
        O += "";

        bool f = true;
        auto sink = [&](const ProtocolParsing::ProtocolNode &node) {
            if (!f) {
                O += "";
            }
            f = false;
            O += on_proto(node);
        };
        _view.protos_iterate(sink);

        for (auto &line : O) {
            auto white = [](auto c) {
                switch (c) {
                    case ' ':
                    case '\n':
                        return true;
                }
                return false;
            };
            auto non_space = std::ranges::find_if_not(line, white);
            if (non_space == std::end(line)) {
                line.clear();
            }
        }

        return O;
    }

    std::optional<std::string_view> to_cxx_type(std::string_view type)
    {
        using Pair = std::pair<std::string_view, std::string_view>;
        Pair map[]{
            {"int", "std::int_least32_t"},
            {"uint", "std::uint_least32_t"},
            {"new_id", "std::uint_least32_t"},
            {"object", "std::uint_least32_t"},
            {"string", "std::string_view"},
            {"fixed", "std::uint_least32_t"},
            {"array", "std::span<const std::byte>"},
            {"fd", "void *"},
        };

        auto matched = [&](const auto &s) { return type == s.first; };
        auto mapped = std::ranges::find_if(map, matched);
        if (mapped == std::end(map)) {
            return {};
        }
        return mapped->second;
    }

    LineList on_proto(const ProtocolParsing::ProtocolNode &proto_node)
    {
        LineList O;
        if (!proto_node.interfaces) {
            return O;
        }

        auto name = proto_node.name.value();
        O += std::format("struct {}", name);
        O += "{";

        std::vector<
            std::reference_wrapper<const ProtocolParsing::InterfaceNode>>
            iface_nodes;

        auto sink = [&](const ProtocolParsing::Node &node) {
            const auto &iface_node =
                std::get<ProtocolParsing::InterfaceNode>(node);
            iface_nodes.push_back(std::ref(iface_node));
        };
        _view.chain_iterate(proto_node.interfaces.value(), sink);

        bool f = true;
        for (const ProtocolParsing::InterfaceNode &iface_node : iface_nodes) {
            std::string_view iface_name = iface_node.name.value();
            std::string iface_typename{iface_name};
            if (iface_typename == name) {
                iface_typename = std::format("{}_interface", iface_typename);
            }
            if (!f) {
                O += "";
            }
            f = false;
            O += define_iface(iface_node, iface_typename);
        }

        O += std::format("}}; // struct {}", name);

        O += "namespace wlalat";
        O += "{";

        f = true;
        for (const ProtocolParsing::InterfaceNode &iface_node : iface_nodes) {
            std::string_view iface_name = iface_node.name.value();
            std::string iface_typename{iface_name};
            if (iface_typename == name) {
                iface_typename = std::format("{}_interface", iface_typename);
            }
            if (!f) {
                O += "";
            }
            f = false;
            O += define_iface_traits(
                name, iface_node, iface_typename, iface_name);
        }

        O += "} // namespace wlalat";

        return O;
    }

    LineList define_iface(
        const ProtocolParsing::InterfaceNode &iface_node, std::string_view name)
    {
        LineList O;

        O += std::format("struct {}", name);
        O += "{";

        std::vector<std::reference_wrapper<const ProtocolParsing::RequestNode>>
            requests;
        std::vector<std::reference_wrapper<const ProtocolParsing::EventNode>>
            events;

        auto sink = [&](const ProtocolParsing::Node &node) {
            const auto *req_node =
                std::get_if<ProtocolParsing::RequestNode>(&node);
            if (req_node) {
                requests.push_back(*req_node);
            }

            const auto *ev_node =
                std::get_if<ProtocolParsing::EventNode>(&node);
            if (ev_node) {
                events.push_back(*ev_node);
            }
        };
        if (iface_node.requests) {
            _view.chain_iterate(iface_node.requests.value(), sink);
        }

        if (iface_node.events) {
            _view.chain_iterate(iface_node.events.value(), sink);
        }

        bool req_has_fd = false;
        for (size_t i = 0; i != requests.size(); ++i) {
            auto opcode = i;
            std::list<AmogusArg> args;
            const ProtocolParsing::RequestNode &req = requests[i];
            if (req.args) {
                args = AmogusArg::collect_amogusified(req.args.value(), _view);
            }
            req_has_fd = req_has_fd || has_fd(args);
            O += gen_request(req, args, opcode);
        }

        bool ev_has_fd = false;
        for (size_t i = 0; i != events.size(); ++i) {
            auto opcode = i;
            const ProtocolParsing::EventNode &ev = events[i];
            std::list<AmogusArg> args;
            if (ev.args) {
                args = AmogusArg::collect_amogusified(ev.args.value(), _view);
            }
            O += gen_event(ev, args, opcode);
            ev_has_fd = ev_has_fd || has_fd(args);
        }

        O += std::format("}}; // struct {}", name);

        return O;
    }

    LineList define_iface_traits(
        std::string_view proto_ns,
        const ProtocolParsing::InterfaceNode &iface_node,
        std::string_view iface_typename,
        std::string_view iface_name)
    {
        LineList O;

        std::vector<std::reference_wrapper<const ProtocolParsing::RequestNode>>
            requests;
        std::vector<std::reference_wrapper<const ProtocolParsing::EventNode>>
            events;

        auto sink = [&](const ProtocolParsing::Node &node) {
            const auto *req_node =
                std::get_if<ProtocolParsing::RequestNode>(&node);
            if (req_node) {
                requests.push_back(*req_node);
            }

            const auto *ev_node =
                std::get_if<ProtocolParsing::EventNode>(&node);
            if (ev_node) {
                events.push_back(*ev_node);
            }
        };
        if (iface_node.requests) {
            _view.chain_iterate(iface_node.requests.value(), sink);
        }

        if (iface_node.events) {
            _view.chain_iterate(iface_node.events.value(), sink);
        }

        LineList requests_msg_types;
        for (size_t idx = 0; idx != requests.size(); ++idx) {
            const ProtocolParsing::RequestNode &req = requests[idx];
            std::list<AmogusArg> args;
            auto &req_name = req.name.value();
            if (req.args) {
                args = AmogusArg::collect_amogusified(req.args.value(), _view);
            }
            requests_msg_types += req_name;
            bool is_event = false;
            size_t opcode = idx;
            O += gen_args_traits(
                proto_ns, iface_typename, req_name, args, opcode, is_event);
        }

        LineList events_msg_types;
        for (size_t idx = 0; idx != events.size(); ++idx) {
            const ProtocolParsing::EventNode &ev = events[idx];
            std::list<AmogusArg> args;
            auto &ev_name = ev.name.value();
            if (ev.args) {
                args = AmogusArg::collect_amogusified(ev.args.value(), _view);
            }
            events_msg_types += ev_name;
            size_t opcode = idx;
            bool is_event = true;
            O += gen_args_traits(
                proto_ns, iface_typename, ev_name, args, opcode, is_event);
        }

        O += "template<>";

        O += std::format("struct Traits<{}::{}>", proto_ns, iface_typename);
        O += "{";
        LineList B0;
        B0 += std::format(
            "static constexpr std::string_view name = \"{}\";", iface_name);
        B0 += std::format("using Events = std::tuple");
        B0 += "<";
        LineList B1;
        for (auto &msg_type_name : events_msg_types) {
            std::string full_qualified_msg_type = std::format(
                "{}::{}::message_{}", proto_ns, iface_typename, msg_type_name);
            B1 += std::format("{}", full_qualified_msg_type);
        }
        comma_sep(B1);
        B1.indent();
        B0 += std::move(B1);
        B0 += ">;";

        B0 += std::format("using Requests = std::tuple");
        B0 += "<";
        B1.clear();
        for (auto &msg_type_name : requests_msg_types) {
            std::string full_qualified_msg_type = std::format(
                "{}::{}::message_{}", proto_ns, iface_typename, msg_type_name);
            B1 += std::format("{}", full_qualified_msg_type);
        }
        comma_sep(B1);
        B1.indent();
        B0 += std::move(B1);
        B0 += ">;";

        B0.indent();
        O += std::move(B0);
        O += "};";

        return O;
    }

    LineList gen_args_traits(
        std::string_view proto_ns,
        std::string_view iface_typename,
        std::string_view msg_name,
        const std::list<AmogusArg> &args,
        size_t opcode,
        bool is_event)
    {
        LineList O;

        O += "template<>";

        std::string full_qualified_msg_type = std::format(
            "{}::{}::message_{}", proto_ns, iface_typename, msg_name);
        O += std::format("struct Traits<{}>", full_qualified_msg_type);
        O += "{";

        LineList B0;

        B0 += std::format("using Type = {};", full_qualified_msg_type);
        B0 += std::format(
            "using InterfaceTag = {}::{};", proto_ns, iface_typename);
        B0 += std::format(
            "static constexpr std::string_view name = \"{}\";", msg_name);
        B0 += std::format("static constexpr const size_t opcode = {};", opcode);
        B0 += std::format("static constexpr bool is_event = {};", is_event);

        LineList B1;

        std::vector<std::string_view> arg_names;
        std::vector<std::string_view> arg_types;
        for (auto &arg : args) {
            std::string_view arg_name{arg.name.value()};
            arg_names.push_back(arg_name);
            auto &arg_type = arg.type.value();
            arg_types.push_back(arg_type);
        }

        std::vector<std::array<std::string, 3>> meta_entries;
        {
            for (auto name_type_pair : std::views::zip(arg_names, arg_types)) {
                auto name = std::get<0>(name_type_pair);
                auto type = std::get<1>(name_type_pair);
                std::array<std::string, 3> entry;
                entry[0] = std::format("typename TypeTagsT::wl_{}{{}}", type);
                entry[1] = std::format("&Type::{}", name);
                entry[2] = std::format("\"{}\"", name);
                meta_entries.push_back(std::move(entry));
            }

            std::array<size_t, 3> meta_entry_max_lens{};
            for (auto &meta_entry : meta_entries) {
                auto &max = meta_entry_max_lens;
                max[0] = std::max(max[0], meta_entry[0].size());
                max[1] = std::max(max[1], meta_entry[1].size());
                max[2] = std::max(max[2], meta_entry[2].size());
            }

            for (auto &meta_entry : meta_entries) {
                meta_entry[0].resize(meta_entry_max_lens[0], ' ');
                meta_entry[1].resize(meta_entry_max_lens[1], ' ');
                meta_entry[2].resize(meta_entry_max_lens[2], ' ');
            }
        }

        B0 += "template<typename TypeTagsT>";
        B0 += "static constexpr auto args_meta = std::make_tuple";
        B0 += "(";
        B1.clear();
        for (auto &meta_entry : meta_entries) {
            B1 += std::format(
                "std::make_tuple({},{},{})",
                meta_entry[0],
                meta_entry[1],
                meta_entry[2]);
        }
        comma_sep(B1);
        B1.indent();
        B0 += std::move(B1);
        B0 += ");";

        B0.indent();
        O += std::move(B0);
        O += "};";
        return O;
    }

    LineList gen_request(
        const ProtocolParsing::RequestNode &req,
        std::list<AmogusArg> &args,
        size_t opcode)
    {
        return gen_message(req.name, args, opcode);
    }

    LineList gen_event(
        const ProtocolParsing::EventNode &ev,
        std::list<AmogusArg> &args,
        size_t opcode)
    {
        return gen_message(ev.name, args, opcode);
    }

    static bool has_fd(const std::list<AmogusArg> &args)
    {
        bool has_fd = false;
        auto &am_args = args;
        auto is_fd = [](const AmogusArg &arg) {
            if (!arg.type) {
                return false;
            }
            return arg.type.value() == "fd";
        };
        return std::ranges::find_if(am_args, is_fd) != std::end(am_args);
    }

    LineList gen_message(
        const ProtocolParsing::AttrString &name_op,
        std::list<AmogusArg> &args,
        size_t opcode)
    {
        LineList O;

        auto name = name_op.value();

        O += std::format("struct message_{}", name);
        O += "{";
        LineList B;
        for (auto &arg : args) {
            B += define_arg(arg);
        }

        B.indent();
        O += std::move(B);
        O += "};";
        return O;
    }

    LineList define_arg(const ProtocolParsing::ArgRawTag &arg)
    {
        LineList O;
        auto type = arg.type.value();
        auto type_str = std::format("void * /* UNMAPPED {} */", type);
        auto cxx_type_str_op = to_cxx_type(type);
        if (cxx_type_str_op) {
            type_str = std::format("{}", cxx_type_str_op.value());
        }

        O += std::format("{} {};", type_str, arg.name.value());
        return O;
    }

  private:
    ProtocolParsing::ProtocolTreeView _view;
};
}; // namespace CodeGen
}; // namespace wlalat
