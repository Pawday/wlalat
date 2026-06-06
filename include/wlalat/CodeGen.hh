#pragma once

#include "ProtocolParser.hh"

#include <cstddef>

#include <algorithm>
#include <format>
#include <functional>
#include <optional>
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
        O += "#include <wlalat/Message.hh>";
        O += "#include <wlalat/Parser.hh>";
        O += "#include <wlalat/Types.hh>";
        O += "#include <wlalat/Writer.hh>";
        O += "";
        O += "#include <cstddef>";
        O += "";
        O += "#include <optional>";
        O += "#include <type_traits>";
        O += "#include <variant>";
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
            {"new_id", "/* new_id */ wlalat::UInt"},
            {"id", "/* id */ wlalat::UInt"},
            {"object", "/* object */ wlalat::UInt"},
            {"string", "wlalat::String"},
            {"fixed", "wlalat::Fixed"},
            {"array", "wlalat::Array"},
            {"fd", "void * /* fd */"},
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
        O += std::format("namespace {}", name);
        O += "{";

        bool f = true;
        auto sink = [&](const ProtocolParsing::Node &node) {
            if (!f) {
                O += "";
            }
            f = false;

            const auto &iface_node =
                std::get<ProtocolParsing::InterfaceNode>(node);
            O += define_iface(iface_node);
        };
        _view.chain_iterate(proto_node.interfaces.value(), sink);

        O += std::format("}} // namespace {}", name);
        return O;
    }

    LineList define_iface(const ProtocolParsing::InterfaceNode &iface_node)
    {
        LineList O;
        auto name = iface_node.name.value();

        O += std::format("namespace {}", name);
        O += "{";

        O += std::format("namespace message");
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

        for (size_t i = 0; i != requests.size(); ++i) {
            auto opcode = i;
            auto &req = requests[i];
            O += gen_request(req, opcode);
        }

        for (size_t i = 0; i != events.size(); ++i) {
            auto opcode = i;
            auto &ev = events[i];
            O += gen_event(ev, opcode);
        }

        O += std::format("}} // namespace message");

        O += "";
        O += gen_variant(events, "Event");
        O += gen_variant(requests, "Request");
        O += std::format("}} // namespace {}", name);

        return O;
    }

    template <typename MessageNodeT>
    LineList gen_variant(
        const std::vector<std::reference_wrapper<const MessageNodeT>> &msgs,
        std::string_view class_name)
    {
        LineList O;

        if (msgs.empty()) {
            return O;
        }

        O += std::format("struct {} : std::variant<", class_name);

        LineList B0;
        std::optional<std::string_view> prev;
        for (const MessageNodeT &msg : msgs) {
            if (prev) {
                B0 += std::format("message::{},", prev.value());
            }
            prev = msg.name.value();
        }
        if (prev) {
            B0 += std::format("message::{}", prev.value());
        }
        B0.indent();
        O += std::move(B0);

        LineList B1;

        O += ">";
        O += "{";
        B0.clear();
        B0 += std::format(
            "static std::optional<{}> parse(wlalat::MessageView M)",
            class_name);
        B0 += "{";
        B1 += std::format("wlalat::Parser P{{M.payload}};");

        std::vector<std::reference_wrapper<const ProtocolParsing::ArgNode>>
            args;
        for (const MessageNodeT &msg : msgs) {
            auto &name = msg.name.value();
            std::string opcode_ref = std::format("message::{}::opcode", name);

            auto sink = [&](const ProtocolParsing::Node &node) {
                auto &arg_node = std::get<ProtocolParsing::ArgNode>(node);
                args.push_back(std::ref(arg_node));
            };

            args.clear();
            if (msg.args) {
                _view.chain_iterate(msg.args.value(), sink);
            }

            LineList if_body;
            if_body += std::format("message::{} O;", name);
            if_body += gen_read_body(args);
            if_body += std::format("return {}{{O}};", class_name);

            B1 += std::format("if (M.opcode == {}) {{", opcode_ref);
            if_body.indent();
            B1 += std::move(if_body);
            B1 += std::format("}}");
        }

        B1 += "return {};";

        B1.indent();
        B0 += std::move(B1);
        B0 += "}";
        B0.indent();
        O += std::move(B0);

        B0.clear();
        B0 += gen_variant_write_with_visitor(msgs);
        B0.indent();
        O += "";
        O += std::move(B0);

        O += "};";
        return O;
    }

    LineList gen_request(const ProtocolParsing::RequestNode &req, size_t opcode)
    {
        return gen_message(req.name, req.args, opcode);
    }

    LineList gen_event(const ProtocolParsing::EventNode &ev, size_t opcode)
    {
        return gen_message(ev.name, ev.args, opcode);
    }

    template <typename MessageNodeT>
    LineList gen_variant_write_with_visitor(
        const std::vector<std::reference_wrapper<const MessageNodeT>> &msgs)
    {
        LineList O;
        O += gen_write_visitor(msgs);
        O += "";
        O += "template<typename OIterT>";
        O += "void write(wlalat::Writer<OIterT> &W)";
        O += "{";
        LineList B0;
        B0 += "std::visit(WriteVisitor<OIterT>{W},*this);";
        B0.indent();
        O += std::move(B0);
        O += "}";
        return O;
    }

    template <typename MessageNodeT>
    LineList gen_write_visitor(
        const std::vector<std::reference_wrapper<const MessageNodeT>> &msgs)
    {
        LineList O;
        O += "template<typename OIterT>";
        O += "struct WriteVisitor";
        O += "{";

        LineList B0;
        B0 += "wlalat::Writer<OIterT> &W;";
        B0 += "";

        bool f = true;
        for (const MessageNodeT &msg : msgs) {
            auto name = msg.name.value();
            if (!f) {
                B0 += "";
            }
            f = false;
            B0 += gen_write_visitor_overload(name, msg.args);
        }

        B0.indent();
        O += std::move(B0);
        O += "};";
        return O;
    }

    LineList gen_write_visitor_overload(
        const ProtocolParsing::AttrString &name_op,
        const std::optional<
            ProtocolParsing::IndexChainNode<ProtocolParsing::ArgNode>>
            &args_chain_start)
    {
        LineList O;
        auto name = name_op.value();

        std::vector<std::reference_wrapper<const ProtocolParsing::ArgNode>>
            args;

        auto sink = [&](const ProtocolParsing::Node &node) {
            auto &arg_node = std::get<ProtocolParsing::ArgNode>(node);
            args.push_back(std::ref(arg_node));
        };
        if (args_chain_start) {
            _view.chain_iterate(args_chain_start.value(), sink);
        }

        O += std::format("void operator()(const message::{} &M)", name);
        O += "{";
        LineList B0 = gen_write_body(args);
        B0.indent();
        O += std::move(B0);
        O += "}";
        return O;
    }

    LineList gen_message(
        const ProtocolParsing::AttrString &name_op,
        const std::optional<
            ProtocolParsing::IndexChainNode<ProtocolParsing::ArgNode>> &args,
        size_t opcode)
    {
        LineList O;

        auto name = name_op.value();

        O += std::format("struct {}", name);
        O += "{";
        LineList B;
        B += std::format("static constexpr const size_t opcode = {};", opcode);

        bool f = true;
        auto define_sink = [&](const ProtocolParsing::Node &node) {
            if (!f) {
                B += "";
            }
            f = false;

            const auto &arg_node = std::get<ProtocolParsing::ArgNode>(node);
            B += define_arg(arg_node);
        };

        if (args) {
            _view.chain_iterate(args.value(), define_sink);
        }

        B.indent();
        O += std::move(B);

        O += "};";
        O += "";

        O += std::format(
            "inline std::optional<{}> read_{}(wlalat::MessageView M)",
            name,
            name);
        O += "{";
        LineList body;
        body += std::format("wlalat::Parser P{{M.payload}};");
        body += std::format("{} O;", name);

        std::vector<std::reference_wrapper<const ProtocolParsing::ArgNode>>
            arg_nodes;
        auto arg_node_sink = [&](const ProtocolParsing::Node &node) {
            const auto &arg_node = std::get<ProtocolParsing::ArgNode>(node);
            arg_nodes.push_back(std::ref(arg_node));
        };
        if (args) {
            _view.chain_iterate(args.value(), arg_node_sink);
        }

        body += gen_read_body(arg_nodes);

        body += "return O;";
        body.indent();
        O += std::move(body);
        O += "}";

        body.clear();
        O += "";
        O += "template<typename OIterT>";
        O += std::format(
            "void write(const {} &M, wlalat::Writer<OIterT> W)", name, name);
        O += "{";

        body.clear();
        body = gen_write_body(arg_nodes);
        body.indent();
        O += std::move(body);
        O += "}";

        return O;
    }

    LineList gen_read_body(
        std::vector<std::reference_wrapper<const ProtocolParsing::ArgNode>>
            arg_nodes)
    {
        LineList body;

        for (const ProtocolParsing::ArgNode &arg_node : arg_nodes) {
            auto &B = body;
            auto N = arg_node.name.value();

            bool is_fd = false;
            if (arg_node.type && arg_node.type.value() == "fd") {
                is_fd = true;
            }

            if (is_fd) {
                B += "/* Ignore fd";
            }

            B += std::format(
                "if (!P.has(std::type_identity<decltype(O.{})>{{}})) return "
                "{{}};",
                N);
            B += std::format(
                "O.{} = P.next(std::type_identity<decltype(O.{})>{{}});", N, N);

            if (is_fd) {
                B += "*/";
            }
        }
        return body;
    }

    LineList gen_write_body(
        std::vector<std::reference_wrapper<const ProtocolParsing::ArgNode>>
            arg_nodes)
    {
        LineList body;
        for (const ProtocolParsing::ArgNode &arg_node : arg_nodes) {
            auto &B = body;
            auto N = arg_node.name.value();

            bool is_fd = false;
            if (arg_node.type && arg_node.type.value() == "fd") {
                is_fd = true;
            }

            if (is_fd) {
                B += "/* Ignore fd";
            }

            B += std::format("W.write(M.{});", N);

            if (is_fd) {
                B += "*/";
            }
        }
        return body;
    }

    LineList define_arg(const ProtocolParsing::ArgNode &arg)
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
