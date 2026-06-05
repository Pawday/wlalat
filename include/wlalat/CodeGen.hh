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
        O += gen_event_dispatcher(events);
        O += std::format("}} // namespace {}", name);

        return O;
    }

    LineList gen_event_dispatcher(
        const std::vector<
            std::reference_wrapper<const ProtocolParsing::EventNode>> &events)
    {
        LineList O;
        O += "struct EventDispatcher";
        O += "{";

        LineList dispatch_b;
        for (const ProtocolParsing::EventNode &ev : events) {
            auto &name = ev.name.value();
            std::string opcode_ref = std::format("message::{}::opcode", name);

            LineList if_body;

            if_body += std::format("auto op = message::read_{}(M);", name);
            if_body += "if (!op) return;";
            if_body += "on(op.value());";

            dispatch_b += std::format("if (M.opcode == {}) {{", opcode_ref);
            if_body.indent();
            dispatch_b += std::move(if_body);
            dispatch_b += std::format("}}");
        }

        LineList b;
        b += "void dispatch(const wlalat::MessageView &M)";
        b += "{";
        dispatch_b.indent();
        b += std::move(dispatch_b);
        b += "}";

        b += "";

        for (const ProtocolParsing::EventNode &ev : events) {
            auto &name = ev.name.value();
            b += std::format("virtual void on(const message::{} &) = 0;", name);
        }

        b.indent();
        O += std::move(b);
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
        auto read_body = [&](const ProtocolParsing::Node &node) {
            const auto &arg_node = std::get<ProtocolParsing::ArgNode>(node);

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
        };
        if (args) {
            _view.chain_iterate(args.value(), read_body);
        }
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

        auto write_body = [&](const ProtocolParsing::Node &node) {
            const auto &arg_node = std::get<ProtocolParsing::ArgNode>(node);

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
        };

        if (args) {
            _view.chain_iterate(args.value(), write_body);
        }
        body.indent();
        O += std::move(body);
        O += "}";

        return O;
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
