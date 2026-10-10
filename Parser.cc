#include <wlalat/codegen/CodeGen.hh>
#include <wlalat/codegen/Converter.hh>
#include <wlalat/codegen/Format.hh>
#include <wlalat/protocol/ExpatParser.hh>
#include <wlalat/codegen/Types.hh>

#include <cstddef>

#include <exception>
#include <format>
#include <fstream>
#include <ios>
#include <print>
#include <vector>

int main(int argc, char **argv)
try {
    if (argc < 2) {
        throw std::invalid_argument{"Bad argc"};
    }
    std::ifstream file{argv[1]};
    file >> std::noskipws;
    file.exceptions(std::ios::badbit);
    auto is = std::views::istream<char>(file);

    wlalat::Protocol::ExpatParser p;

    for (auto c : is) {
        p.send(c);
    }

    std::vector<wlalat::CodeGen::Protocol> protos_collect;
    wlalat::CodeGen::Converter::convert(p.finalize(), protos_collect);

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
