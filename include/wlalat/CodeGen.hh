#pragma once

#include "ProtocolParser.hh"

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
    bool f = true;
    for (auto &line : std::views::reverse(lines)) {
        if (f) {
            f = false;
            continue;
        }
        line.append(",");
    }
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
        O += "#include <wlalat/Types.hh>";
        O += "";
        O += "#include <cstddef>";
        O += "";
        O += "#include <array>";
        O += "#include <string_view>";
        O += "#include <variant>";
        O += "#include <tuple>";
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

    std::optional<std::string_view> to_wlalat_type(std::string_view type)
    {
        using Pair = std::pair<std::string_view, std::string_view>;

        Pair map[]{
            {"int", "wlalat::Int"},
            {"uint", "wlalat::UInt"},
            {"new_id", "wlalat::NewID"},
            {"object", "wlalat::Object"},
            {"string", "wlalat::String"},
            {"fixed", "wlalat::Fixed"},
            {"array", "wlalat::Array"},
            {"fd", "FDT"},
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

        bool requests_has_fd = false;
        LineList requests_msg_types_may_be_FDT;
        for (size_t idx = 0; idx != requests.size(); ++idx) {
            const ProtocolParsing::RequestNode &req = requests[idx];
            std::list<AmogusArg> args;
            auto &req_name = req.name.value();
            if (req.args) {
                args = AmogusArg::collect_amogusified(req.args.value(), _view);
            }
            bool arg_with_fd = has_fd(args);
            const char *fdt_param = arg_with_fd ? "<FDT>" : "";
            requests_msg_types_may_be_FDT +=
                std::format("{}{}", req_name, fdt_param);
            requests_has_fd = requests_has_fd || arg_with_fd;
            bool is_event = false;
            size_t opcode = idx;
            O += gen_args_traits(
                proto_ns, iface_typename, req_name, args, opcode, is_event);
        }

        bool events_has_fd = false;
        LineList events_msg_types_may_be_FDT;
        for (size_t idx = 0; idx != events.size(); ++idx) {
            const ProtocolParsing::EventNode &ev = events[idx];
            std::list<AmogusArg> args;
            auto &ev_name = ev.name.value();
            if (ev.args) {
                args = AmogusArg::collect_amogusified(ev.args.value(), _view);
            }
            bool arg_with_fd = has_fd(args);
            const char *fdt_param = arg_with_fd ? "<FDT>" : "";
            events_msg_types_may_be_FDT +=
                std::format("{}{}", ev_name, fdt_param);
            events_has_fd = events_has_fd || arg_with_fd;
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
        if (events_has_fd) {
            B0 += std::format("template<typename FDT>");
        }
        B0 += std::format("using Event = std::variant");
        B0 += "<";
        LineList B1;
        for (auto &msg_type_name : events_msg_types_may_be_FDT) {
            std::string full_qualified_msg_type = std::format(
                "{}::{}::message_{}", proto_ns, iface_typename, msg_type_name);
            B1 += std::format("{}", full_qualified_msg_type);
        }
        if (events_msg_types_may_be_FDT.empty()) {
            B1 += "std::monostate";
        }
        comma_sep(B1);
        B1.indent();
        B0 += std::move(B1);
        B0 += ">;";

        if (requests_has_fd) {
            B0 += std::format("template<typename FDT>");
        }
        B0 += std::format("using Request = std::variant");
        B0 += "<";
        B1.clear();
        for (auto &msg_type_name : requests_msg_types_may_be_FDT) {
            std::string full_qualified_msg_type = std::format(
                "{}::{}::message_{}", proto_ns, iface_typename, msg_type_name);
            B1 += std::format("{}", full_qualified_msg_type);
        }
        if (requests_msg_types_may_be_FDT.empty()) {
            B1 += "std::monostate";
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

        bool with_fd = has_fd(args);
        if (!with_fd) {
            O += "template<>";
        } else {
            O += "template<typename FDT>";
        }

        const char *fdt_template_param = "";
        if (with_fd) {
            fdt_template_param = "<FDT>";
        }
        std::string full_qualified_msg_type = std::format(
            "{}::{}::message_{}{}",
            proto_ns,
            iface_typename,
            msg_name,
            fdt_template_param);
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

        std::set<std::string_view> arg_wlalat_types_unique;
        for (auto &arg : args) {
            auto arg_type = arg.type.value();
            auto arg_wlalat_type = to_wlalat_type(arg_type).value();
            arg_wlalat_types_unique.emplace(arg_wlalat_type);
        }

        LineList B1;

        std::vector<std::string_view> arg_names;
        for (auto &arg : args) {
            auto arg_name = arg.name.value();
            arg_names.push_back(arg_name);
        }

        B0 += "using ArgMemberPointerTuple = std::tuple";
        B0 += "<";
        B1.clear();
        for (auto &name : arg_names) {
            B1 += std::format("decltype(&Type::{})", name);
        }
        comma_sep(B1);
        B1.indent();
        B0 += std::move(B1);
        B0 += ">;";

        B0 += "static constexpr ArgMemberPointerTuple args_tuple";
        B0 += "{";
        B1.clear();
        for (auto &name : arg_names) {
            B1 += std::format("&Type::{}", name);
        }
        comma_sep(B1);
        B1.indent();
        B0 += std::move(B1);
        B0 += "};";

        B0 += std::format(
            "static constexpr std::array<std::string_view, {}> arg_names",
            arg_names.size());
        B0 += "{";
        B1.clear();
        for (auto &name : arg_names) {
            B1 += std::format("\"{}\"", name);
        }
        comma_sep(B1);
        B1.indent();
        B0 += std::move(B1);
        B0 += "};";

        B1.clear();

        for (auto &arg : args) {
            auto arg_type = arg.type.value();
            auto arg_wlalat_type = to_wlalat_type(arg_type).value();
            arg_wlalat_types_unique.emplace(arg_wlalat_type);
        }

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

        if (has_fd(args)) {
            O += "template<typename FDT>";
        }
        O += std::format("struct message_{}", name);
        O += "{";
        LineList B;

        if (true) {
            bool f = true;
            for (auto &arg : args) {
                if (!f) {
                    B += "";
                }
                f = false;
                B += define_arg(arg);
            }
        }

        B.indent();
        O += std::move(B);

        O += "};";
        return O;
    }

    LineList gen_write_body(
        std::vector<std::reference_wrapper<const ProtocolParsing::ArgRawTag>>
            args)
    {
        LineList body;
        for (const ProtocolParsing::ArgRawTag &arg : args) {
            auto &B = body;
            auto N = arg.name.value();
            B += std::format("W(M.{});", N);
        }
        return body;
    }

    LineList define_arg(const ProtocolParsing::ArgRawTag &arg)
    {
        LineList O;

        if (arg.summary) {
            O += std::format("/* {} */", arg.summary.value());
        }

        auto type = arg.type.value();
        auto type_str = std::format("void * /* UNMAPPED {} */", type);
        auto wlalat_type_str_op = to_wlalat_type(type);
        if (wlalat_type_str_op) {
            type_str = std::format("{}", wlalat_type_str_op.value());
        }

        O += std::format("{} {};", type_str, arg.name.value());
        return O;
    }

  private:
    ProtocolParsing::ProtocolTreeView _view;
};
}; // namespace CodeGen
}; // namespace wlalat
