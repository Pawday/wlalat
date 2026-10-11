#include <wlalat/codegen/Types.hh>
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
#include <vector>

int main(int argc, char **argv)
try {
    namespace V = std::views;

    if (argc < 0) {
        throw std::invalid_argument{"Bad argc"};
    }

    std::vector<std::string> args_vec;
    for (size_t i = 0; i != argc; ++i) {
        args_vec.push_back(argv[i]);
    }

    auto args = args_vec | V::drop(1);
    if (args.size() < 2) {
        throw std::invalid_argument{
            "Bad args: expected input(s) and single output"};
    }
    auto in_files_args = args | V::take(args.size() - 1);
    auto o_file_arg = (args | V::drop(args.size() - 1)).front();

    std::vector<wlalat::CodeGen::Protocol> protos;

    for (auto file_arg : in_files_args) {
        std::ifstream file{file_arg};
        file >> std::noskipws;
        file.exceptions(std::ios::badbit);
        auto is = std::views::istream<char>(file);

        wlalat::CodeGen::ProtocolParser p;
        for (char c : is) {
            p.send(c);
        }

        auto parsed = p.parse();
        for (auto &proto : parsed) {
            protos.push_back(std::move(proto));
        }
    }

    wlalat::CodeGen::Generator m{std::move(protos)};
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
