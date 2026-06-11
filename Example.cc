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
#include <optional>
#include <print>
#include <span>
#include <stdexcept>
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
    template <typename TagT>
    struct ID : wlalat::UInt
    {
    };

    template <typename TagT>
    ID<TagT> allocate()
    {
        auto O = _next_free;
        _next_free = wlalat::UInt{O.raw() + 1};
        return ID<TagT>{O};
    }

  private:
    wlalat::UInt _next_free{2};
};

struct MessageOwner
{
    template <typename MsgPayloadT>
    wlalat::MessageView prepare(wlalat::UInt object_id, const MsgPayloadT &P)
    {
        _payload.clear();
        wlalat::Writer w{std::back_inserter(_payload)};
        P.write(w);
        wlalat::MessageView O;
        O.object_id = object_id;
        O.opcode = P.opcode();
        O.payload = _payload;
        return O;
    }

  private:
    std::vector<std::byte> _payload;
};

struct Display
{
    static constexpr const wlalat::UInt hardcoded_display_id{1};

    Display(wlalat::Unix::Socket &s, ObjectIDManager &id_manager)
        : _s{s}, _id_manager{id_manager}
    {
    }

    wlalat::MessageView encode(wayland::wl_display::Request m)
    {
        return _raw_msg.prepare(hardcoded_display_id, m);
    }

    void sync()
    {
        wayland::wl_display::message_sync msg;
        msg.callback = _id_manager.allocate<wayland::wl_callback::Tag>();
        wayland::wl_display::Request req{msg};
        _s.send(encode(req));
    }

    void dispatch(wlalat::MessageView M)
    {
        if (M.object_id != hardcoded_display_id) {
            return;
        }

        wlalat::Parser P{M.payload};

        auto ev_op = wayland::wl_display::Event::parse(P, M.opcode);
        if (!ev_op) {
            return;
        }

        auto vis = [&]<typename EvT>(const EvT &ev) { on(ev); };
        std::visit(vis, ev_op.value());
    }

    void on(const wayland::wl_display::message_error &m)
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

    void on(const wayland::wl_display::message_delete_id &m)
    {
        uint32_t object_id = m.id;
        std::println("Display delete_id MSG: object_id=[{}]", object_id);
    }

  private:
    wlalat::Unix::Socket &_s;
    ObjectIDManager &_id_manager;

    MessageOwner _raw_msg;
};

struct ShmPool : ObjectIDManager::ID<wayland::wl_shm_pool::Tag>
{
    using Tag = wayland::wl_shm_pool::Tag;

    ShmPool(wlalat::Unix::Socket &s, ObjectIDManager &id_manager)
        : ObjectIDManager::ID<Tag>{id_manager.allocate<Tag>()}, _s{s},
          _id_manager{id_manager}
    {
    }

  private:
    wlalat::Unix::Socket &_s;
    MessageOwner _raw_msg;
    ObjectIDManager &_id_manager;
};

struct Shm : ObjectIDManager::ID<wayland::wl_shm::Tag>
{
    using Tag = wayland::wl_shm::Tag;

    Shm(wlalat::Unix::Socket &s, ObjectIDManager &id_manager)
        : ObjectIDManager::ID<Tag>{id_manager.allocate<Tag>()}, _s{s},
          _id_manager{id_manager}
    {
    }

    void create_pool_once()
    {
        if (pool) {
            return;
        }

        pool.emplace(_s, _id_manager);

        wayland::wl_shm::message_create_pool<int> msg;
        msg.fd = 0xAB0BA;
        msg.id = pool.value();
        msg.size = wlalat::Int{-1};

        wayland::wl_shm::Request<int> req{msg};
        wlalat::MessageView req_msg = _raw_msg.prepare(*this, req);
        std::println("Req message_create_pool {}", dump_message(req_msg));
        _s.send(req_msg);
    }

  private:
    wlalat::Unix::Socket &_s;
    ObjectIDManager &_id_manager;

    MessageOwner _raw_msg;
    std::optional<ShmPool> pool;
};

struct Registry : ObjectIDManager::ID<wayland::wl_registry::Tag>
{
    Registry(
        wlalat::Unix::Socket &s,
        ObjectIDManager::ID<wayland::wl_registry::Tag> id,
        ObjectIDManager &id_manager)
        : ObjectIDManager::ID<wayland::wl_registry::Tag>{id}, _s{s},
          _id_manager{id_manager}
    {
    }

    void dispatch(wlalat::MessageView M)
    {
        if (M.object_id != *this) {
            return;
        }
        wlalat::Parser P{M.payload};
        auto ev_op = wayland::wl_registry::Event::parse(P, M.opcode);
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

    void on(const wayland::wl_registry::message_global &msg)
    {
        std::println(
            "{} {} {}",
            static_cast<uint32_t>(msg.name),
            static_cast<std::string_view>(msg.interface),
            static_cast<uint32_t>(msg.version));

        if (msg.interface == "wl_shm") {
            if (shm) {
                throw std::runtime_error{"Duplicate wl_shm global interface"};
            }

            shm.emplace(_s, _id_manager);

            wayland::wl_registry::message_bind bind_msg;
            bind_msg.name = msg.name;
            bind_msg.id_interface_name_amogus_arg = msg.interface;
            bind_msg.id_interface_version_amogus_arg = msg.version;
            bind_msg.id = shm.value();
            wayland::wl_registry::Request req{bind_msg};
            wlalat::MessageView req_msg = _raw_msg.prepare(*this, req);
            std::println("Req shm {}", dump_message(req_msg));
            _s.send(req_msg);
        }
    }

    void on(const wayland::wl_registry::message_global_remove &)
    {
    }

    std::optional<Shm> shm;

  private:
    wlalat::Unix::Socket &_s;
    MessageOwner _raw_msg;
    ObjectIDManager &_id_manager;
};

int main()
try {
    ObjectIDManager id_manager;
    wlalat::Unix::Socket s;

    Display display{s, id_manager};

    auto registry_tag = id_manager.allocate<wayland::wl_registry::Tag>();
    Registry registry{s, registry_tag, id_manager};

    wayland::wl_display::message_get_registry m{};
    m.registry = registry_tag;

    wlalat::MessageView raw_m = display.encode({m});
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

        if (registry.shm) {
            auto &v = registry.shm.value();
            v.create_pool_once();
            display.sync();
        }
    }

} catch (std::exception &e) {
    std::println(stderr, "std::exception [{}]", e.what());
    return EXIT_FAILURE;
}
