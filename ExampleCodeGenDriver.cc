#include <wlalat/CodeGen.hh>
#include <wlalat/ProtocolParser.hh>

#include <cstddef>
#include <cstdlib>

#include <exception>
#include <format>
#include <fstream>
#include <ios>
#include <iterator>
#include <print>
#include <stdexcept>
#include <string>

int main(int argc, char **argv)
try {
    if (argc < 3) {
        throw std::invalid_argument{"Bad argc"};
    }

    std::string file_arg{argv[1]};
    std::string o_file_arg{argv[2]};

    std::ifstream file{file_arg};
    file.exceptions(std::ios::badbit);
    file.exceptions(std::ios::failbit);

    std::string content{
        std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>()};

    wlalat::ProtocolParsing::ProtocolParser p{content};
    wlalat::ProtocolParsing::ProtocolTree tree = p.parse();

    wlalat::CodeGen::Generator m{tree.view()};
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
