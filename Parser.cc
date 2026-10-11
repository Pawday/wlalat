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
    p.finalize();

    wlalat::Protocol::Protocol &proto = p.proto().value();

    std::print("{}", wlalat::CodeGen::Converter::convert(proto));

} catch (std::exception &e) {
    std::println("std::exception::what() [{}]", e.what());
}
