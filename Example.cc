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
#include <cstring>

#include <chrono>
#include <exception>
#include <format>
#include <optional>
#include <print>
#include <span>
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

        const char *hacky_registry_inteface_name =
            reinterpret_cast<const char *>(msg.payload.data());
        hacky_registry_inteface_name += 8;

        std::println("{}", hacky_registry_inteface_name);
    }

} catch (std::exception &e) {
    std::println(stderr, "std::exception [{}]", e.what());
    return EXIT_FAILURE;
}
