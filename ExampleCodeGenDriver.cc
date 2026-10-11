#include <wlalat/codegen/CodeGen.hh>
#include <wlalat/codegen/ProtocolParser.hh>

#include <cstddef>
#include <cstdlib>

#include <exception>
#include <format>
#include <fstream>
#include <ios>
#include <iterator>
#include <print>
#include <ranges>
#include <stdexcept>
#include <string>
#include <utility>

int main(int argc, char **argv)
try {
    if (argc < 3) {
        throw std::invalid_argument{"Bad argc"};
    }

    std::string file_arg{argv[1]};
    std::string o_file_arg{argv[2]};

    std::ifstream file{file_arg};
    file >> std::noskipws;
    file.exceptions(std::ios::badbit);
    auto is = std::views::istream<char>(file);

    wlalat::CodeGen::ProtocolParser p;
    for (char c : is) {
        p.send(c);
    }

    auto protos = p.parse();
    auto &proto = protos.at(0);

    wlalat::CodeGen::Generator m{std::move(proto)};
    auto gen = m.generate();
    std::string gen_data;
    for (auto &l : gen) {
        std::format_to(std::back_inserter(gen_data), "{}\n", l);
    }

    if (o_file_arg == "-") {
        std::println("{}", gen_data);
        return EXIT_SUCCESS;
    }

    std::ofstream of{o_file_arg};
    of << gen_data;

} catch (std::exception &e) {
    std::println("std::exception::what() [{}]", e.what());
    return EXIT_FAILURE;
}
