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
#include <variant>
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

std::string dump_message(const wlalat::MessageView &msg)
{
    return std::format(
        "MSG: obj=[{}], opcode=[{}], {}",
        msg.object_id,
        msg.opcode,
        hexdump(msg.payload));
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
    const wlalat::MessageView &operator()() const
    {
        return _msg;
    }

    wlalat::MessageView &operator()()
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
    wlalat::MessageView _msg;
    std::vector<std::byte> _payload;
};

struct Display
{
    static constexpr const wlalat::UInt hardcoded_display_id{1};

    wlalat::MessageView active_message()
    {
        return _raw_msg();
    }

    void encode(wayland::wl_display::Request m)
    {
        _raw_msg().object_id = hardcoded_display_id;
        _raw_msg().opcode = m.opcode();
        auto w = _raw_msg.writer();
        m.write(w);
        _raw_msg.update_payload();
    }

    void dispatch(wlalat::MessageView M)
    {
        if (M.object_id != hardcoded_display_id) {
            return;
        }

        auto ev_op = wayland::wl_display::Event::parse(M);
        if (!ev_op) {
            return;
        }

        auto vis = [&]<typename EvT>(const EvT &ev) { on(ev); };
        std::visit(vis, ev_op.value());
    }

    void on(const wayland::wl_display::message::error &m)
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

    void on(const wayland::wl_display::message::delete_id &)
    {
    }

  private:
    MessageOwner _raw_msg;
};

struct Registry
{
    wlalat::MessageView active_message()
    {
        return _raw_msg();
    }

    void dispatch(wlalat::MessageView M)
    {
        if (M.object_id != id()) {
            return;
        }
        auto ev_op = wayland::wl_registry::Event::parse(M);
        if (!ev_op) {
            return;
        }

        auto &ev = ev_op.value();

        std::vector<std::byte> p2;
        wlalat::Writer W{std::back_inserter(p2)};
        ev.write(W);

        auto vis = [&]<typename EvT>(const EvT &ev) { on(ev); };
        std::visit(vis, ev_op.value());
        std::println("                          {}", hexdump(p2));
    }

    void on(const wayland::wl_registry::message::global &msg)
    {
        std::println(
            "{} {} {}",
            static_cast<uint32_t>(msg.name),
            static_cast<std::string_view>(msg.interface),
            static_cast<uint32_t>(msg.version));
    }

    void on(const wayland::wl_registry::message::global_remove &)
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

int main()
try {
    wlalat::Unix::Socket s;
    Display display;

    ObjectIDManager id_manager;

    Registry registry;
    registry.id() = id_manager.allocate();

    wayland::wl_display::message::get_registry m{};
    m.registry = registry.id();

    display.encode({m});
    wlalat::MessageView raw_m = display.active_message();
    std::println("{}", dump_message(raw_m));
    s.send(raw_m);

    auto last_message = std::chrono::steady_clock::now();

    while (true) {
        auto elapsed = decltype(last_message)::clock::now() - last_message;
        if (elapsed > message_timeout) {
            break;
        }
        std::optional<wlalat::MessageView> message_op = s.recv();
        if (!message_op) {
            std::this_thread::yield();
            continue;
        }
        wlalat::MessageView msg = message_op.value();
        last_message = decltype(last_message)::clock::now();
        std::println("{}", dump_message(msg));

        display.dispatch(msg);
        registry.dispatch(msg);
    }

} catch (std::exception &e) {
    std::println(stderr, "std::exception [{}]", e.what());
    return EXIT_FAILURE;
}
