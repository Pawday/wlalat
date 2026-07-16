#pragma once

#include "CodeGenInfo.hh"
#include "ProtocolParser.hh"

#include <array>
#include <cstddef>

#include <algorithm>
#include <format>
#include <list>
#include <optional>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <utility>
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
struct AmogusArg : Argument
{
    AmogusArg(const Argument &arg) : Argument{arg}
    {
    }

    static std::list<AmogusArg>
        collect_amogusified(const std::vector<CodeGen::Argument> args)
    {
        std::list<AmogusArg> O;
        for (auto &arg : args) {
            auto name = arg.name.value();
            auto type = arg.type.value();

            bool is_amogus_new_id =
                type == "new_id" && !arg.interface.has_value();
            if (is_amogus_new_id) {

                Argument am_tag{};
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
        }
        return O;
    }

    AmogusArg(const AmogusArg &) = delete;
    AmogusArg(AmogusArg &&) = delete;
    AmogusArg &operator=(const AmogusArg &) = delete;
    AmogusArg &operator=(AmogusArg &&) = delete;
};

struct Generator
{
    Generator(std::vector<CodeGen::Protocol> &&protocols)
        : _protocols{std::move(protocols)}
    {
    }

    LineList generate()
    {
        LineList O;

        O += "#pragma once";
        O += "";
        O += "#include <cstddef>";
        O += "#include <cstdint>";
        O += "";
        O += "#include <string_view>";
        O += "#include <tuple>";
        O += "#include <span>";
        O += "";

        bool f = true;
        for (auto &proto : _protocols) {
            O += on_proto(proto);
        }

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

    LineList on_proto(const CodeGen::Protocol &proto_node)
    {
        LineList O;
        if (proto_node.interfaces.empty()) {
            return O;
        }

        auto name = proto_node.name.value();
        O += std::format("struct {}", name);
        O += "{";

        bool f = true;
        for (auto &iface_node : proto_node.interfaces) {
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

        return O;
    }

    LineList define_iface(
        const CodeGen::Interface &iface_node, std::string_view name)
    {
        LineList O;

        auto &interface_name = name;
        O += std::format("struct {}", name);
        O += "{";

        auto &requests = iface_node.requests;
        auto &events = iface_node.events;

        for (size_t i = 0; i != requests.size(); ++i) {
            auto opcode = i;
            std::list<AmogusArg> args;
            const Request &req = iface_node.requests[i];
            if (!req.args.empty()) {
                args = AmogusArg::collect_amogusified(req.args);
            }
            O += gen_request(interface_name, req, args, opcode);
        }

        for (size_t i = 0; i != events.size(); ++i) {
            auto opcode = i;
            const Event &ev = events[i];
            std::list<AmogusArg> args;
            if (!ev.args.empty()) {
                args = AmogusArg::collect_amogusified(ev.args);
            }
            O += gen_event(interface_name, ev, args, opcode);
        }

        auto meta = gen_iface_meta(iface_node, name);
        O += std::move(meta);

        O += std::format("}}; // struct {}", name);

        return O;
    }

    LineList
        gen_iface_meta(const Interface &iface_node, std::string_view iface_name)
    {
        LineList O;
        O += "struct Meta";
        O += "{";
        LineList B0 = define_iface_meta_content(iface_node, "", iface_name);
        B0.indent();
        O += std::move(B0);
        O += "};";
        return O;
    }

    LineList define_iface_meta_content(
        const Interface &iface_node,
        std::string_view message_type_prefix,
        std::string_view iface_name)
    {
        auto requests = iface_node.requests;
        auto events = iface_node.events;

        LineList requests_msg_types;
        for (size_t idx = 0; idx != requests.size(); ++idx) {
            const Request &req = requests[idx];
            auto &req_name = req.name.value();
            requests_msg_types += req_name;
        }

        LineList events_msg_types;
        for (size_t idx = 0; idx != events.size(); ++idx) {
            const Event &ev = events[idx];
            auto &ev_name = ev.name.value();
            events_msg_types += ev_name;
        }

        LineList B0;
        B0 += std::format(
            "static constexpr std::string_view name = \"{}\";", iface_name);
        B0 += std::format("using Events = std::tuple");
        B0 += "<";
        LineList B1;
        for (auto &msg_type_name : events_msg_types) {
            B1 +=
                std::format("{}message_{}", message_type_prefix, msg_type_name);
        }
        comma_sep(B1);
        B1.indent();
        B0 += std::move(B1);
        B0 += ">;";

        B0 += std::format("using Requests = std::tuple");
        B0 += "<";
        B1.clear();
        for (auto &msg_type_name : requests_msg_types) {
            B1 +=
                std::format("{}message_{}", message_type_prefix, msg_type_name);
        }
        comma_sep(B1);
        B1.indent();
        B0 += std::move(B1);
        B0 += ">;";
        return B0;
    }

    LineList gen_args_meta(
        std::string_view msg_name,
        const std::list<AmogusArg> &args,
        size_t opcode,
        bool is_event,
        std::string_view type_using,
        std::string_view interface_type_using)
    {
        LineList O;
        O += "struct Meta";
        O += "{";
        LineList B0 = gen_args_meta_content(
            msg_name, args, opcode, is_event, type_using, interface_type_using);
        B0.indent();
        O += std::move(B0);
        O += "};";
        return O;
    }

    LineList gen_args_meta_content(
        std::string_view msg_name,
        const std::list<AmogusArg> &args,
        size_t opcode,
        bool is_event,
        std::string_view type_using,
        std::string_view interface_type_using)
    {
        LineList B0;

        B0 += std::format("using Type = {};", type_using);
        B0 += std::format("using InterfaceTag = {};", interface_type_using);
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
        B0 += "static constexpr auto args = std::make_tuple";
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
        return B0;
    }

    LineList gen_request(
        std::string_view interface_name,
        const Request &req,
        std::list<AmogusArg> &args,
        size_t opcode)
    {
        return gen_message(interface_name, req.name, args, opcode, false);
    }

    LineList gen_event(
        std::string_view interface_name,
        const Event &ev,
        std::list<AmogusArg> &args,
        size_t opcode)
    {
        return gen_message(interface_name, ev.name, args, opcode, true);
    }

    LineList gen_message(
        std::string_view interface_name,
        const ProtocolParsing::AttrString &name_op,
        std::list<AmogusArg> &args,
        size_t opcode,
        bool is_event)
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

        auto type_using = std::format("message_{}", name);
        B = gen_args_meta(
            name, args, opcode, is_event, type_using, interface_name);
        B.indent();
        O += std::move(B);
        O += "};";
        return O;
    }

    LineList define_arg(const Argument &arg)
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
    std::vector<CodeGen::Protocol> _protocols;
};
}; // namespace CodeGen
}; // namespace wlalat
