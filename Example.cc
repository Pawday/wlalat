#include <wlalat/Binary.hh>
#include <wlalat/Error.hh>
#include <wlalat/Message.hh>
#include <wlalat/MessageParser.hh>
#include <wlalat/MessageSerializer.hh>
#include <wlalat/Unix/Socket.hh>

#include <sys/socket.h>
#include <sys/un.h>

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <chrono>
#include <exception>
#include <format>
#include <optional>
#include <print>
#include <span>
#include <string_view>
#include <thread>
#include <vector>

constexpr std::chrono::seconds message_timeout{1};

constexpr uint32_t wl_display_object_id = 1;
constexpr uint32_t wl_display_get_registry_request_id = 1;

namespace wlalat {

struct StringParser
{
    StringParser(std::span<const std::byte> data) : _data{data}
    {
    }

    struct Layout
    {
        std::span<const std::byte> string_data;
        std::span<const std::byte> undefined_padding;

        uint_least64_t message_size() const
        {
            return 4 + string_data.size() + undefined_padding.size();
        };

        auto string_data_zero_term() const
            -> std::optional<std::span<const std::byte>>
        {
            auto O = string_data;
            if (O.empty()) {
                return {};
            }

            if (O.back() != std::byte{0}) {
                return {};
            }

            return O;
        }

        auto string_view() const -> std::optional<std::basic_string_view<char>>
        {
            auto data_zterm_op = string_data_zero_term();
            if (!data_zterm_op) {
                return {};
            }
            auto data_zterm = data_zterm_op.value();
            data_zterm = data_zterm.subspan(0, data_zterm.size() - 1);

            const char *start =
                reinterpret_cast<const char *>(data_zterm.data());
            const char *end = start + data_zterm.size();
            return std::basic_string_view<char>{start, end};
        }
    };

    std::optional<Layout> parse() const
    {
        if (_data.size() < 4) {
            return {};
        }
        auto head = _data.subspan(4);
        uint_least32_t string_size_with_zeroterm = fle32(_data.subspan<0, 4>());

        if (head.size() < string_size_with_zeroterm) {
            return {};
        }
        Layout O{};
        O.string_data = head.subspan(0, string_size_with_zeroterm);
        head = head.subspan(string_size_with_zeroterm);

        uint_least8_t undefined_padding_size = 0;
        if (string_size_with_zeroterm % 4 != 0) {
            undefined_padding_size = 4 - (string_size_with_zeroterm % 4);
        }

        if (head.size() < undefined_padding_size) {
            return {};
        }
        O.undefined_padding = head.subspan(0, undefined_padding_size);
        return O;
    }

  private:
    std::span<const std::byte> _data;
};

struct wl_registry_message_global
{
    /* numeric name of the global object */
    uint32_t name;

    /* interface implemented by the object */
    std::string_view interface;

    /* interface version */
    uint32_t version;

    static auto parse(Message msg) -> std::optional<wl_registry_message_global>
    {
        wl_registry_message_global O{};
        auto parse_span = msg.payload;

        if (parse_span.size() < 4) {
            return {};
        }
        O.name = fle32(parse_span.subspan<0, 4>());
        parse_span = parse_span.subspan(4);

        StringParser interface_string_parser{parse_span};
        auto interface_string_layout_op = interface_string_parser.parse();
        if (!interface_string_layout_op) {
            return {};
        }
        auto interface_string_layout = interface_string_layout_op.value();
        auto interface_string_op = interface_string_layout.string_view();
        if (!interface_string_op) {
            return {};
        }

        O.interface = interface_string_op.value();
        parse_span = parse_span.subspan(interface_string_layout.message_size());

        if (parse_span.size() < 4) {
            return {};
        }
        O.version = fle32(parse_span.subspan<0, 4>());

        return O;
    }
};

}; // namespace wlalat

int main()
try {
    wlalat::Unix::Socket s;

    wlalat::Message m;
    m.object_id = wl_display_object_id;
    m.opcode = wl_display_get_registry_request_id;
    uint32_t wl_registry_id = wl_display_object_id + 1;
    auto wl_registry_id_data = wlalat::tole32(wl_registry_id);
    m.payload = wl_registry_id_data;

    s.send(m);

    std::pmr::vector<std::byte> active_message_buf;

    auto last_message = std::chrono::steady_clock::now();

    while (true) {
        auto elapsed = decltype(last_message)::clock::now() - last_message;
        if (elapsed > message_timeout) {
            break;
        }
        std::optional<wlalat::Message> message_op = s.recv(active_message_buf);
        if (!message_op) {
            std::this_thread::yield();
            continue;
        }
        wlalat::Message msg = message_op.value();
        last_message = decltype(last_message)::clock::now();

        auto global_msg_op = wlalat::wl_registry_message_global::parse(msg);
        auto &global_msg = global_msg_op.value();

        std::println(
            "{} {} {}",
            global_msg.name,
            global_msg.interface,
            global_msg.version);
    }

} catch (std::exception &e) {
    std::println(stderr, "std::exception [{}]", e.what());
    return EXIT_FAILURE;
}
