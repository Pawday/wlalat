#include <wlalat/codegen/CodeGen.hh>
#include <wlalat/codegen/Converter.hh>
#include <wlalat/codegen/Format.hh>
#include <wlalat/protocol/ExpatParser.hh>
#include <wlalat/codegen/Types.hh>

#include <cstddef>
#include <cstdlib>

#include <exception>
#include <format>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <optional>
#include <print>
#include <ranges>
#include <stdexcept>
#include <string>

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

    std::optional<std::ofstream> ofile_stream;
    std::ostream_iterator<char> O = [&]() {
        if (o_file_arg == "-") {
            return std::ostream_iterator<char>{std::cout, ""};
        }
        ofile_stream.emplace(o_file_arg);
        return std::ostream_iterator<char>{ofile_stream.value(), ""};
    }();

    bool f = true;
    std::format_to(O, "[");
    for (auto file_arg : in_files_args) {

        std::ifstream file{file_arg};
        file >> std::noskipws;
        file.exceptions(std::ios::badbit);
        auto if_stream = std::istreambuf_iterator<char>{file};

        wlalat::Protocol::ExpatParser p;

        auto if_stream_char =
            std::ranges::subrange(if_stream, std::istreambuf_iterator<char>());
        for (char c : if_stream_char) {
            p.send(c);
        }
        p.finalize();

        auto &proto_op = p.proto();
        auto &proto = proto_op.value();
        auto cg_proto = wlalat::CodeGen::Converter::convert(proto);

        if (!f) {
            std::format_to(O, ",");
        }
        f = false;
        std::format_to(O, "{}", cg_proto);
    }
    std::format_to(O, "]");
} catch (std::exception &e) {
    std::println("std::exception::what() [{}]", e.what());
    return EXIT_FAILURE;
}
