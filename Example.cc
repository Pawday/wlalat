#include "GeneratedTests.hh"

#include <wlalat/Binary.hh>
#include <wlalat/Error.hh>
#include <wlalat/Message.hh>
#include <wlalat/MessageParser.hh>
#include <wlalat/MessageSerializer.hh>
#include <wlalat/Parser.hh>
#include <wlalat/StringParser.hh>
#include <wlalat/Types.hh>
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

        auto global_msg_op = Generated::wl_registry::message::as_global(msg);
        auto &global_msg = global_msg_op.value();

        std::println(
            "{} {} {}",
            static_cast<uint32_t>(global_msg.name),
            static_cast<std::string_view>(global_msg.interface),
            static_cast<uint32_t>(global_msg.version));
    }

} catch (std::exception &e) {
    std::println(stderr, "std::exception [{}]", e.what());
    return EXIT_FAILURE;
}
