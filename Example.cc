#include "wayland.xml.hh"

#include <wlalat/Binary.hh>
#include <wlalat/Error.hh>
#include <wlalat/Message.hh>
#include <wlalat/MessageParser.hh>
#include <wlalat/MessageSerializer.hh>
#include <wlalat/Parser.hh>
#include <wlalat/StringParser.hh>
#include <wlalat/Types.hh>
#include <wlalat/Unix/Socket.hh>
#include <wlalat/Writer.hh>

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
#include <iterator>
#include <memory_resource>
#include <optional>
#include <print>
#include <span>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

constexpr std::chrono::seconds message_timeout{1};

constexpr uint32_t wl_display_object_id = 1;

std::string hexdump(std::span<const std::byte> data)
{
    std::string O;
    O += '[';
    auto f = true;
    for (auto b : data) {
        if (!f) {
            O += ',';
        }
        f = false;
        uint16_t bi = std::to_integer<uint16_t>(b);
        std::format_to(std::back_inserter(O), "{:02x}", bi);
    }
    O += ']';
    return O;
}

int main()
try {
    auto addr = wlalat::Unix::Socket::make_address(1000, 1);
    wlalat::Unix::Socket s{addr, std::pmr::get_default_resource()};

    wayland::wl_display::message::get_registry m{};
    m.registry = wlalat::UInt{2};
    std::vector<std::byte> payload;
    wlalat::Writer w{std::back_inserter(payload)};
    wayland::wl_display::message::write_get_registry(m, w);

    wlalat::Message raw_m;
    raw_m.object_id = wl_display_object_id;
    raw_m.opcode = m.opcode;
    raw_m.payload = payload;
    std::println("{}", hexdump(payload));
    s.send(raw_m);

    auto last_message = std::chrono::steady_clock::now();

    while (true) {
        auto elapsed = decltype(last_message)::clock::now() - last_message;
        if (elapsed > message_timeout) {
            break;
        }
        std::optional<wlalat::Message> message_op = s.recv();
        if (!message_op) {
            std::this_thread::yield();
            continue;
        }
        wlalat::Message msg = message_op.value();
        last_message = decltype(last_message)::clock::now();

        auto global_msg_op = wayland::wl_registry::message::read_global(msg);
        auto &global_msg = global_msg_op.value();

        std::println(
            "{} {} {}",
            static_cast<uint32_t>(global_msg.name),
            static_cast<std::string_view>(global_msg.interface),
            static_cast<uint32_t>(global_msg.version));

        std::vector<std::byte> reencoded_payload;
        wlalat::Writer w{std::back_inserter(reencoded_payload)};
        wayland::wl_registry::message::write_global(global_msg, w);
        std::println("{}", hexdump(msg.payload));
        std::println("{}", hexdump(reencoded_payload));
    }

} catch (std::exception &e) {
    std::println(stderr, "std::exception [{}]", e.what());
    return EXIT_FAILURE;
}
