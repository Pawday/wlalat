#include <wlalat/ProtocolParser.hh>

#include <cstddef>

#include <exception>
#include <fstream>
#include <ios>
#include <iterator>
#include <print>
#include <string>
#include <string_view>

constexpr auto test(std::string_view str)
{
    wlalat::ProtocolParsing::ProtocolParser p{str};
    auto tree = p.parse();
    return tree.size();
}

static_assert(test("<protocol>") == 1);
static_assert(test("<protocol><interface>") == 2);

int main(int argc, char **argv)
try {
    std::ifstream file{argv[1]};
    file.exceptions(std::ios::badbit);
    file.exceptions(std::ios::failbit);

    std::string content{
        std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>()};

    wlalat::ProtocolParsing::ProtocolParser p{content};

} catch (std::exception &e) {
    std::println("std::exception::what() [{}]", e.what());
}

#if 0
        if not consteval {
            auto interfaces = chain_nodes(proto.interfaces.value());
            for (const InterfaceNode &el : interfaces) {
                std::println("__AA__ [{}]", el.name.value_or("?"));
                if (el.requests) {
                    auto reqs = chain_nodes(el.requests.value());
                    for (const RequestNode &req : reqs) {
                        std::println(
                            "__AA__     req [{}]", req.name.value_or("?"));

                        if (req.args) {
                            auto args = chain_nodes(req.args.value());
                            for (const ArgNode &arg : args) {
                                std::println(
                                    "__AA__         arg [{}]",
                                    arg.name.value_or("?"));
                            }
                        }
                    }
                }

                if (el.events) {
                    auto evs = chain_nodes(el.events.value());
                    for (const EventNode &ev : evs) {
                        std::println(
                            "__AA__     ev [{}]", ev.name.value_or("?"));

                        if (ev.args) {
                            auto args = chain_nodes(ev.args.value());
                            for (const ArgNode &arg : args) {
                                std::println(
                                    "__AA__         arg [{}]",
                                    arg.name.value_or("?"));
                            }
                        }
                    }
                }

                if (el.enums) {
                    auto ens = chain_nodes(el.enums.value());
                    for (const EnumNode &en : ens) {
                        std::println(
                            "__AA__     en [{}]", en.name.value_or("?"));

                        if (en.entries) {
                            auto ens = chain_nodes(en.entries.value());
                            for (const EntryNode &ent : ens) {
                                std::println(
                                    "__AA__         enr [{}] [{}]",
                                    ent.name.value_or("?"),
                                    ent.value.value_or("?"));
                            }
                        }
                    }
                }
            }
        }
#endif
