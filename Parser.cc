#include <wlalat/ProtocolParser.hh>

#include <cstddef>

#include <exception>
#include <fstream>
#include <ios>
#include <iterator>
#include <print>
#include <string>

int main(int argc, char **argv)
try {
    std::ifstream file{argv[1]};
    file.exceptions(std::ios::badbit);
    file.exceptions(std::ios::failbit);

    std::string content{
        std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>()};

    wlalat::ProtocolParser p{content};

    for (size_t i = 0; i != 10000; ++i) {
        p.test_process();
    }

    auto tags = p.test_tags();
    for (auto &t : tags) {
        std::println("{}", t);
    }

} catch (std::exception &e) {
    std::println("std::exception::what() [{}]", e.what());
}
