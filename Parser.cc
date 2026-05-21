#include <wlalat/ProtocolParser.hh>

#include <fstream>
#include <iterator>
#include <string>

int main(int argc, char **argv)
{
    std::ifstream file{argv[1]};
    file.exceptions(std::ios::badbit);
    file.exceptions(std::ios::failbit);

    std::string content{
        std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>()};

    wlalat::ProtocolParser p{content};

    for (size_t i = 0; i != 10000; ++i) {
        p.test_process();
    }
}
