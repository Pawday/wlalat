#include <wlalat/Binary.hh>
#include <wlalat/Error.hh>
#include <wlalat/Message.hh>
#include <wlalat/MessageParser.hh>
#include <wlalat/MessageSerializer.hh>
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

namespace wlalat {

struct Parser
{
    Parser(std::span<const std::byte> data) : _data{data}
    {
    }

    template <typename T> bool has() const = delete;
    // clang-format off
    template <> bool has<Numeric>()  const { return _data.size() >= 4; }
    template <> bool has<Int>()      const { return has<Numeric>(); }
    template <> bool has<UInt>()     const { return has<Numeric>(); }
    template <> bool has<Fixed>()    const { return has<Numeric>(); }
    // clang-format on

    template <> bool has<String>() const
    {
        StringParser p{_data};
        std::optional<StringParser::Layout> parsed_op = p.parse();
        if (!parsed_op) {
            return false;
        }
        StringParser::Layout parsed = parsed_op.value();
        return parsed_op.has_value();
    }

    template <typename T> T next() = delete;
    template <> Numeric next()
    {
        std::span<const std::byte, 4> d = _data.subspan<0, 4>();
        _data = _data.subspan(4);
        return Numeric{fle32(d)};
    }

    // clang-format off
    template <> Int   next() { return Int   { next<Numeric>()}; };
    template <> UInt  next() { return UInt  { next<Numeric>()}; };
    template <> Fixed next() { return Fixed { next<Numeric>()}; };
    // clang-format on

    template <> String next()
    {
        StringParser p{_data};
        StringParser::Layout parsed = p.parse().value();
        String O = parsed.string().value();
        _data = _data.subspan(parsed.message_size());
        return O;
    }

private:
    std::span<const std::byte> _data;
};

struct wl_registry_message_global
{
    /* numeric name of the global object */
    wlalat::UInt name;

    /* interface implemented by the object */
    wlalat::String interface;

    /* interface version */
    wlalat::UInt version;

    static auto parse(
        wlalat::Message M) -> std::optional<wl_registry_message_global>
    {
        wlalat::Parser p{M.payload};
        wl_registry_message_global O{};

        using name_t = decltype(O.name);
        if (!p.has<name_t>()) { return {}; }
        O.name = p.next<name_t>();

        using interface_t = decltype(O.interface);
        if(!p.has<interface_t>()) { return {}; }
        O.interface = p.next<interface_t>();

        using version_t = decltype(O.version);
        if(!p.has<version_t>()) { return {}; };
        O.version = p.next<version_t>();

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

        auto global_msg_op = wlalat::wl_registry_message_global::parse(msg);
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
