#pragma once

#include "ProtocolParser.hh"

#include <cstddef>

#include <algorithm>
#include <format>
#include <functional>
#include <list>
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
        O += "#include <wlalat/Types.hh>";
        O += "";
        O += "#include <cstddef>";
        O += "#include <cstdint>";
        O += "";
        O += "#include <optional>";
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

        O += "struct Tag {};";

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
                B0 += std::format("message_{},", prev.value());
            }
            prev = msg.name.value();
        }
        if (prev) {
            B0 += std::format("message_{}", prev.value());
        }
        B0.indent();
        O += std::move(B0);

        LineList B1;

        O += ">";
        O += "{";
        B0.clear();
        B0 += std::format("template<typename ParserT>");
        B0 += std::format(
            "static std::optional<{}> parse(ParserT &P, uint_least16_t opcode)",
            class_name);
        B0 += "{";

        std::vector<std::reference_wrapper<const ProtocolParsing::ArgRawTag>>
            args;
        for (const MessageNodeT &msg : msgs) {
            auto &name = msg.name.value();
            std::string opcode_ref = std::format("message_{}::opcode", name);

            std::list<AmogusArg> am_args;
            args.clear();
            if (msg.args) {
                am_args =
                    AmogusArg::collect_amogusified(msg.args.value(), _view);
                for (auto &arg : am_args) {
                    args.push_back(std::ref(arg));
                }
            }

            LineList if_body;
            if_body += std::format("message_{} O;", name);
            if_body += gen_read_body(args);
            if_body += std::format("return {}{{O}};", class_name);

            B1 += std::format("if (opcode == {}) {{", opcode_ref);
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

        B0.clear();
        B0 += "auto opcode() const";
        B0 += "{";
        B1.clear();
        B1 += "auto V = []<typename MsgT>(const MsgT &){return MsgT::opcode;};";
        B1 += "return std::visit(V, *this);";
        B1.indent();
        B0 += std::move(B1);
        B0 += "}";
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
        O += "template<typename WriterT>";
        O += "void write(WriterT &W) const";
        O += "{";
        LineList B0;
        B0 += "std::visit(WriteVisitor<WriterT>{W}, *this);";
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
        O += "template<typename WriterT>";
        O += "struct WriteVisitor";
        O += "{";

        LineList B0;
        B0 += "WriterT &W;";
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

        std::list<AmogusArg> amogus_args;
        std::vector<std::reference_wrapper<const ProtocolParsing::ArgRawTag>>
            args;

        auto sink = [&](const ProtocolParsing::ArgRawTag &arg) {
            amogus_args.emplace_back(arg);
        };
        if (args_chain_start) {
            amogus_args =
                AmogusArg::collect_amogusified(args_chain_start.value(), _view);
        }

        for (auto &am_arg : amogus_args) {
            args.push_back(std::ref(am_arg));
        }

        O += std::format("void operator()(const message_{} &M)", name);
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

        O += std::format("struct message_{}", name);
        O += "{";
        LineList B;
        B += std::format("static constexpr const size_t opcode = {};", opcode);

        if (args) {
            auto am_args = AmogusArg::collect_amogusified(args.value(), _view);
            bool f = true;
            for (auto &arg : am_args) {
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

    LineList gen_read_body(
        std::vector<std::reference_wrapper<const ProtocolParsing::ArgRawTag>>
            args)
    {
        LineList body;

        for (const ProtocolParsing::ArgRawTag &arg : args) {
            auto &B = body;
            auto N = arg.name.value();
            B += std::format("if (!P(O.{})) return {{}};", N);
        }
        return body;
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
