#include <wlalat/CodeGen.hh>
#include <wlalat/CodeGenInfo.hh>
#include <wlalat/ProtocolParser.hh>

#include <cstddef>

#include <exception>
#include <format>
#include <fstream>
#include <functional>
#include <ios>
#include <iterator>
#include <print>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

constexpr auto test(std::string_view str)
{
    wlalat::ProtocolParsing::ProtocolParser p{str};
    auto tree = p.parse();
    return tree.size();
}

static_assert(test("<protocol></protocol>") == 1);
static_assert(
    test(
        "<protocol><interface></interface></protocol><protocol><interface></"
        "interface></protocol>") == 2);

wlalat::CodeGen::LineList
    dump(const std::vector<wlalat::CodeGen::Argument> &args)
{
    wlalat::CodeGen::LineList O;
    for (auto &arg : args) {
        O += std::format(
            "Argument name=[{}] type=[{}] interface=[{}]",
            arg.name.value_or("<null>"),
            arg.type.value_or("<null>"),
            arg.interface.value_or("<null>"));
    }
    return O;
}

wlalat::CodeGen::LineList
    dump(const std::vector<wlalat::CodeGen::EnumEntry> &entries)
{
    wlalat::CodeGen::LineList O;
    for (auto &entry : entries) {
        O += std::format(
            "Entry name=[{}] value=[{}]",
            entry.name.value_or("<null>"),
            entry.value.value_or("<null>"));
    }
    return O;
}

wlalat::CodeGen::LineList dump(const wlalat::CodeGen::Enum &enum_v)
{
    wlalat::CodeGen::LineList O;
    O += std::format("Enum [{}]", enum_v.name.value_or("<null>"));
    auto entries = dump(enum_v.entries);
    entries.indent();
    O += std::move(entries);
    return O;
}

wlalat::CodeGen::LineList dump(const wlalat::CodeGen::Event &ev)
{
    wlalat::CodeGen::LineList O;
    O += std::format("Event [{}]", ev.name.value_or("<null>"));
    auto args = dump(ev.args);
    args.indent();
    O += std::move(args);
    return O;
}

wlalat::CodeGen::LineList dump(const wlalat::CodeGen::Request &req)
{
    wlalat::CodeGen::LineList O;
    O += std::format("Request [{}]", req.name.value_or("<null>"));
    auto args = dump(req.args);
    args.indent();
    O += std::move(args);
    return O;
}

wlalat::CodeGen::LineList dump(const wlalat::CodeGen::Interface &iface)
{
    wlalat::CodeGen::LineList O;
    O += std::format("Interface [{}]", iface.name.value_or("<null>"));

    for (auto &event : iface.events) {
        auto E = dump(event);
        E.indent();
        O += std::move(E);
    }

    for (auto &request : iface.requests) {
        auto R = dump(request);
        R.indent();
        O += std::move(R);
    }

    for (auto &enum_v : iface.enums) {
        auto E = dump(enum_v);
        E.indent();
        O += std::move(E);
    }
    return O;
}

wlalat::CodeGen::LineList dump(const wlalat::CodeGen::Protocol &proto)
{
    wlalat::CodeGen::LineList O;
    O += std::format("Protocol [{}]", proto.name.value_or("<null>"));
    for (auto &iface : proto.interfaces) {
        auto I = dump(iface);
        I.indent();
        O += std::move(I);
    }
    return O;
}

int main(int argc, char **argv)
try {
    std::ifstream file{argv[1]};
    file.exceptions(std::ios::badbit);
    file.exceptions(std::ios::failbit);

    std::string content{
        std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>()};

    wlalat::ProtocolParsing::ProtocolParser p{content};
    auto protos_collect = p.parse();

    for (auto &proto : protos_collect) {
        auto lines = dump(proto);
        for (auto &line : lines) {
            std::println("{}", line);
        }
    }

} catch (std::exception &e) {
    std::println("std::exception::what() [{}]", e.what());
}
