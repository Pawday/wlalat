#pragma once

#include "Types.hh"
#include "LineList.hh"

#include <array>
#include <cstddef>

#include <algorithm>
#include <format>
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

// https://gitlab.freedesktop.org/wayland/wayland/-/commit/85a6a470873357089ffb968a176d5074fddd1756
struct TypelessNewIdArgumentExpander
{
    static auto expand(const std::vector<Argument> &args)
    {
        std::vector<Argument> O;
        for (auto &arg : args) {
            auto name = arg.name.value();
            auto type = arg.type.value();

            bool is_amogus_new_id =
                type == "new_id" && !arg.interface.has_value();
            if (is_amogus_new_id) {

                Argument am_tag{};
                std::string am_name;
                am_name = std::format("{}_interface", name);
                am_tag.name = am_name;
                am_tag.type = "string";
                O.emplace_back(am_tag);

                am_name = std::format("{}_version", name);
                am_tag.name = am_name;
                am_tag.type = "uint";
                O.emplace_back(am_tag);
            }
            O.emplace_back(arg);
        }
        return O;
    }

    static auto expand_at(std::vector<Protocol> &protos)
    {
        auto expand_at_message = []<typename MSG>(MSG &M) {
            M.args = expand(M.args);
        };

        auto expand_at_iface = [&](Interface &iface) {
            std::ranges::for_each(iface.requests, expand_at_message);
        };

        auto exand_at_proto = [&](Protocol &P) {
            std::ranges::for_each(P.interfaces, expand_at_iface);
        };

        std::ranges::for_each(protos, exand_at_proto);
    }
};

struct Generator
{
    Generator(std::vector<CodeGen::Protocol> &&protocols)
        : _protocols{std::move(protocols)}
    {
        TypelessNewIdArgumentExpander::expand_at(_protocols);
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

        O += "template<typename PMeta>";
        O += "struct protocols";
        O += "{";
        for (auto &proto : _protocols) {
            O += on_proto(proto);
        }
        O += "}; // struct protocols";
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
            const Request &req = iface_node.requests[i];
            O += gen_request(interface_name, req, opcode);
        }

        for (size_t i = 0; i != events.size(); ++i) {
            auto opcode = i;
            const Event &ev = events[i];
            O += gen_event(interface_name, ev, opcode);
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
        auto requests = iface_node.requests;
        auto events = iface_node.events;

        LineList B0;
        B0 += std::format("static constexpr char name[] = \"{}\";", iface_name);
        B0 += std::format("using Events = std::tuple");
        B0 += "<";
        LineList B1;
        for (auto &ev : events) {
            auto &msg_type_name = ev.name.value();
            B1 += std::format("message_{}", msg_type_name);
        }
        B1.comma_sep();
        B1.indent();
        B0 += std::move(B1);
        B0 += ">;";

        B0 += std::format("using Requests = std::tuple");
        B0 += "<";
        B1.clear();
        for (auto &req : requests) {
            auto &msg_type_name = req.name.value();
            B1 += std::format("message_{}", msg_type_name);
        }
        B1.comma_sep();
        B1.indent();
        B0 += std::move(B1);
        B0 += ">;";
        B0.indent();
        O += std::move(B0);
        O += "};";
        return O;
    }

    LineList gen_args_meta(
        std::string_view msg_name,
        const std::vector<Argument> &args,
        size_t opcode,
        bool is_event,
        std::string_view type_using,
        std::string_view interface_type_using)
    {
        LineList O;
        O += "struct Meta";
        O += "{";
        LineList B0;

        B0 += std::format("using Type = {};", type_using);
        B0 += std::format("using InterfaceTag = {};", interface_type_using);
        B0 += std::format("static constexpr char name[] = \"{}\";", msg_name);
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
        B1.comma_sep();
        B1.indent();
        B0 += std::move(B1);
        B0 += ");";

        B0.indent();
        O += std::move(B0);
        O += "};";
        return O;
    }

    LineList gen_request(
        std::string_view interface_name, const Request &req, size_t opcode)
    {
        return gen_message(interface_name, req.name, req.args, opcode, false);
    }

    LineList gen_event(
        std::string_view interface_name, const Event &ev, size_t opcode)
    {
        return gen_message(interface_name, ev.name, ev.args, opcode, true);
    }

    LineList gen_message(
        std::string_view interface_name,
        const MetadataEntry &name_op,
        const std::vector<Argument> &args,
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
