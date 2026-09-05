#include <wlalat/codegen/XML.hh>

#include <cstddef>

#include <exception>
#include <format>
#include <fstream>
#include <ios>
#include <iterator>
#include <print>
#include <stdexcept>
#include <string>
#include <string_view>

void xml_parse_test(std::string_view content)
{
    std::string O;

    struct
    {
        void on_tag(std::string_view tag)
        {
            std::print("[=[TAG]=]{}[=[END_TAG]=]", tag);
        }

        void on_data(std::string_view data)
        {
            std::print("[=[DATA]=]{}[=[END_DATA]=]", data);
        }

        void on_comment(std::string_view comment)
        {
            std::print("[=[COMMENT]=]{}[=[END_COMMENT]=]", comment);
        }
    } callback;

    wlalat::xml::Parser P{&callback};

    size_t line_n = 1;

    std::string line;

    bool was_line = false;
    auto on_line = [&](std::string_view line) {
        if (was_line) {
            P.send('\n');
        }
        was_line = true;
        std::print(
            "[=[LINE{:_>4}]=]{}[=[END_LINE{:_>4}]=]", line_n, line, line_n);
        for (auto lc : line) {
            P.send(lc);
        }
    };

    std::optional<std::string> line_op;
    for (auto c : content) {
        if (!line_op) {
            line_op = std::string{};
        }
        auto &line = line_op.value();

        if (c != '\n') {
            line += c;
            continue;
        }

        on_line(line);
        line_op.reset();
        line_n++;
    }

    if (line_op) {
        on_line(line_op.value());
    }

    P.finalize();

    std::print("{}", O);
}

int main(int argc, char **argv)
try {
    if (argc < 2) {
        throw std::runtime_error{"Bad argc"};
    }

    std::ifstream file{argv[1]};
    file.exceptions(std::ios::badbit);
    file.exceptions(std::ios::failbit);

    std::string content{
        std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>()};

    xml_parse_test(content);

} catch (std::exception &e) {
    std::println("std::exception::what() [{}]", e.what());
}
