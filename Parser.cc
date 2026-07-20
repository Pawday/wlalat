#include <wlalat/CodeGen.hh>
#include <wlalat/CodeGenInfo.hh>
#include <wlalat/CodeGenInfoFormat.hh>
#include <wlalat/ProtocolParser.hh>

#include <cstddef>

#include <exception>
#include <format>
#include <fstream>
#include <ios>
#include <print>
#include <string_view>
#include <vector>

constexpr auto test(std::string_view str)
{
    wlalat::CodeGen::ProtocolParser p;
    for (auto c : str) {
        p.send(c);
    }
    auto tree = p.parse();
    return tree.size();
}

static_assert(test("<protocol></protocol>") == 1);
static_assert(
    test("<protocol><interface></interface></protocol><protocol><interface></"
         "interface></protocol>") == 2);

int main(int argc, char **argv)
try {
    std::ifstream file{argv[1]};
    file >> std::noskipws;
    file.exceptions(std::ios::badbit);
    auto is = std::views::istream<char>(file);

    wlalat::CodeGen::ProtocolParser p;

    for (auto c : is) {
        p.send(c);
    }

    auto protos_collect = p.parse();

    bool f = true;
    std::print("[");
    for (auto &proto : protos_collect) {
        if (!f) {
            std::print(",");
        }
        f = false;
        std::print("{}", proto);
    }
    std::print("]");

} catch (std::exception &e) {
    std::println("std::exception::what() [{}]", e.what());
}
