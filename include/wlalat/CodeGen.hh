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
        O += "#include <wlalat/ArgsIterator.hh>";
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
        O += std::format("namespace {}", name);
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
            if (!f) {
                O += "";
            }
            f = false;
            O += define_iface(iface_node);
        }

        O += std::format("}} // namespace {}", name);

        O += "namespace wlalat";
        O += "{";
        f = true;
        for (const ProtocolParsing::InterfaceNode &iface_node : iface_nodes) {
            if (!f) {
                O += "";
            }
            f = false;
            O += define_iface_arg_iterators(name, iface_node);
        }
        O += "} // namespace wlalat";

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

        O += "";
        O += gen_variant(events, ev_has_fd, "Event");
        O += gen_variant(requests, req_has_fd, "Request");
        O += std::format("}} // namespace {}", name);

        return O;
    }

    LineList define_iface_arg_iterators(
        std::string_view proto_ns,
        const ProtocolParsing::InterfaceNode &iface_node)
    {
        LineList O;

        auto &iface_name = iface_node.name.value();

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

        for (const ProtocolParsing::RequestNode &req : requests) {
            std::list<AmogusArg> args;
            auto &req_name = req.name.value();
            if (req.args) {
                args = AmogusArg::collect_amogusified(req.args.value(), _view);
            }
            O += gen_args_iterator(proto_ns, iface_name, req_name, args);
        }

        for (const ProtocolParsing::EventNode &ev : events) {
            std::list<AmogusArg> args;
            auto &req_name = ev.name.value();
            if (ev.args) {
                args = AmogusArg::collect_amogusified(ev.args.value(), _view);
            }
            O += gen_args_iterator(proto_ns, iface_name, req_name, args);
        }

        return O;
    }

    LineList gen_args_iterator(
        std::string_view proto_ns,
        std::string_view iface_name,
        std::string_view msg_name,
        const std::list<AmogusArg> &args)
    {
        LineList O;

        bool with_fd = has_fd(args);

        if (with_fd) {
            O += "template <typename VisitorT, typename FDT>";
        } else {
            O += "template <typename VisitorT>";
        }
        const char *fdt_str = "";
        if (with_fd) {
            fdt_str = "<FDT>";
        }
        O += std::format(
            "struct ArgsIteratorWithName<VisitorT, {}::{}::message_{}{}>",
            proto_ns,
            iface_name,
            msg_name,
            fdt_str);

        LineList SB; // Struct Body
        O += "{";
        SB += "template<typename MsgT>";
        SB += "constexpr ArgsIteratorWithName(VisitorT &V, MsgT &M)";
        SB += "{";

        LineList CB; // Constructor Body

        for (auto &arg : args) {
            auto &arg_name = arg.name.value();
            CB += std::format("V(M.{}, \"{}\");", arg_name, arg_name);
        }
        CB.indent();
        SB += std::move(CB);
        SB += "}";
        SB.indent();
        O += std::move(SB);
        O += "};";
        return O;
    }

    template <typename MessageNodeT>
    LineList gen_variant(
        const std::vector<std::reference_wrapper<const MessageNodeT>> &msgs,
        bool fd_templated,
        std::string_view class_name)
    {
        LineList O;

        if (msgs.empty()) {
            return O;
        }

        if (fd_templated) {
            O += std::format("template<typename FDT>");
        }
        O += std::format("struct {} : std::variant<", class_name);

        LineList B0;
        std::optional<std::string_view> prev;
        bool prev_is_fd = false;
        for (const MessageNodeT &msg : msgs) {
            if (prev) {
                B0 += std::format(
                    "message_{}{},", prev.value(), prev_is_fd ? "<FDT>" : "");
            }
            std::list<AmogusArg> args;
            if (msg.args) {
                args = AmogusArg::collect_amogusified(msg.args.value(), _view);
            }
            prev_is_fd = has_fd(args);
            prev = msg.name.value();
        }
        if (prev) {
            B0 += std::format(
                "message_{}{}", prev.value(), prev_is_fd ? "<FDT>" : "");
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
        B1 += "bool G = true;";
        B1 += "auto V = [&]<typename T>(T &A) {if (!G) return; G = P(A);};";

        std::vector<std::reference_wrapper<const ProtocolParsing::ArgRawTag>>
            args;
        for (const MessageNodeT &msg : msgs) {
            auto &name = msg.name.value();

            std::list<AmogusArg> am_args;
            args.clear();
            if (msg.args) {
                am_args =
                    AmogusArg::collect_amogusified(msg.args.value(), _view);
                for (auto &arg : am_args) {
                    args.push_back(std::ref(arg));
                }
            }

            bool msg_fd_templated = has_fd(am_args);
            std::string opcode_ref = std::format(
                "message_{}{}::opcode", name, msg_fd_templated ? "<FDT>" : "");

            LineList if_body;
            if_body += std::format(
                "message_{}{} O;", name, msg_fd_templated ? "<FDT>" : "");
            if_body += "wlalat::ArgsIterator{V, O};";
            if_body += "if (!G) return {};";
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
        B0 += gen_variant_write(msgs);
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

    template <typename MessageNodeT>
    [[deprecated]] LineList gen_variant_write(
        const std::vector<std::reference_wrapper<const MessageNodeT>> &msgs)
    {
        LineList O;
        O += "template<typename WriterT>";
        O += "[[deprecated]] void write(WriterT &W) const";
        O += "{";
        LineList B0;
        B0 += "auto V = [&]<typename MsgT>(const MsgT &M) { "
              "wlalat::ArgsIterator{W, M}; };";
        B0 += "std::visit(V, *this);";
        B0.indent();
        O += std::move(B0);
        O += "}";
        return O;
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
        B += std::format("static constexpr const size_t opcode = {};", opcode);

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
