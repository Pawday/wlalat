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
#include <print>
#include <ranges>
#include <stdexcept>
#include <string>

int main(int argc, char **argv)
try {
    if (argc < 3) {
        throw std::invalid_argument{"Bad argc"};
    }

    std::string file_arg{argv[1]};
    std::string o_file_arg{argv[2]};

    std::ifstream file;
    std::istreambuf_iterator<char> stream;

    if (file_arg == "-") {
        stream = std::istreambuf_iterator<char>{std::cin};
    } else {
        file = std::ifstream{file_arg};
        file >> std::noskipws;
        file.exceptions(std::ios::badbit);
        stream = std::istreambuf_iterator<char>{file};
    }

    wlalat::Protocol::ExpatParser p;

    auto stream_range =
        std::ranges::subrange(stream, std::istreambuf_iterator<char>());
    for (auto c : stream_range) {
        p.send(c);
    }
    p.finalize();

    wlalat::CodeGen::Protocol proto =
        wlalat::CodeGen::Converter::convert(p.proto().value());

    auto write_proto_to = [&proto](auto &stream) {
        std::ostream_iterator<char> iter{stream, ""};
        std::format_to(iter, "{}", proto);
    };

    if (o_file_arg == "-") {
        write_proto_to(std::cout);
        return EXIT_SUCCESS;
    }

    std::ofstream o_file{o_file_arg};
    write_proto_to(o_file);
    return EXIT_SUCCESS;
} catch (std::exception &e) {
    std::println("std::exception::what() [{}]", e.what());
    return EXIT_FAILURE;
}
