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

struct ObjectIDManager
{
    wlalat::UInt allocate()
    {
        auto O = _next_free;
        _next_free = wlalat::UInt{O.raw() + 1};
        return O;
    }

  private:
    wlalat::UInt _next_free{2};
};

struct MessageOwner
{
    const wlalat::Message &operator()() const
    {
        return _msg;
    }

    wlalat::Message &operator()()
    {
        return _msg;
    }

    auto writer()
    {
        _payload.clear();
        wlalat::Writer w{std::back_inserter(_payload)};
        return w;
    }

    auto update_payload()
    {
        _msg.payload = _payload;
    }

  private:
    wlalat::Message _msg;
    std::vector<std::byte> _payload;
};

struct Display : wayland::wl_display::EventDispatcher
{
    static constexpr const wlalat::UInt hardcoded_display_id{1};

    wlalat::Message active_message()
    {
        return _raw_msg();
    }

    template <typename MsgT>
    void encode(MsgT &m)
    {
        _raw_msg().object_id = hardcoded_display_id;
        _raw_msg().opcode = m.opcode;
        wayland::wl_display::message::write(m, _raw_msg.writer());
        _raw_msg.update_payload();
    }

    void on(const wayland::wl_display::message::error &m) override
    {
        uint32_t object_id = m.object_id;
        uint32_t code = m.code;
        std::string_view message = m.message;
        std::println(
            "Display error MSG: object_id=[{}] code=[{}], message=[{}]",
            object_id,
            code,
            message);
    }

    void on(const wayland::wl_display::message::delete_id &) override
    {
    }

  private:
    MessageOwner _raw_msg;
};

struct Registry : wayland::wl_registry::EventDispatcher
{
    wlalat::Message active_message()
    {
        return _raw_msg();
    }

    template <typename MsgT>
    void encode(MsgT &m)
    {
        _raw_msg().object_id = id();
        _raw_msg().opcode = m.opcode;
        wayland::wl_registry::message::write(m, _raw_msg.writer());
        _raw_msg.update_payload();
    }

    void on(const wayland::wl_registry::message::global &msg) override
    {
        std::println(
            "{} {} {}",
            static_cast<uint32_t>(msg.name),
            static_cast<std::string_view>(msg.interface),
            static_cast<uint32_t>(msg.version));
    }

    void on(const wayland::wl_registry::message::global_remove &) override
    {
    }

    wlalat::UInt &id()
    {
        return _id;
    };

    const wlalat::UInt &id() const
    {
        return _id;
    };

  private:
    wlalat::UInt _id;
    MessageOwner _raw_msg;
};

std::string dump_message(const wlalat::Message &msg)
{
    return std::format(
        "MSG: obj=[{}], opcode=[{}], {}",
        msg.object_id,
        msg.opcode,
        hexdump(msg.payload));
}

int main()
try {
    wlalat::Unix::Socket s;
    Display display;

    ObjectIDManager id_manager;

    Registry registry;
    registry.id() = id_manager.allocate();

    wayland::wl_display::message::get_registry m{};
    m.registry = registry.id();

    display.encode(m);
    wlalat::Message raw_m = display.active_message();
    std::println("{}", dump_message(raw_m));
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
        std::println("{}", dump_message(msg));

        display.dispatch(msg);
        registry.dispatch(msg);
    }

} catch (std::exception &e) {
    std::println(stderr, "std::exception [{}]", e.what());
    return EXIT_FAILURE;
}
