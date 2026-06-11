#include "wayland.xml.hh"

#include <wlalat/Binary.hh>
#include <wlalat/Error.hh>
#include <wlalat/Message.hh>
#include <wlalat/MessageParser.hh>
#include <wlalat/MessageSerializer.hh>
#include <wlalat/MessageViewFD.hh>
#include <wlalat/Parser.hh>
#include <wlalat/StringParser.hh>
#include <wlalat/Types.hh>
#include <wlalat/Unix/Socket.hh>
#include <wlalat/Writer.hh>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

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
    wlalat::MessageViewFD<int>
        prepare(wlalat::UInt object_id, const MsgPayloadT &P)
    {
        _payload.clear();
        _fd_payload.clear();
        wlalat::Writer w{std::back_inserter(_payload)};

        WriterFDInterceptor wfd{w, _fd_payload};
        P.write(wfd);
        wlalat::MessageViewFD<int> O;
        O.object_id = object_id;
        O.opcode = P.opcode();
        O.payload = _payload;
        O.fds = _fd_payload;
        return O;
    }

  private:
    template <typename UpstreamWriterT>
    struct WriterFDInterceptor
    {
        UpstreamWriterT &_w;
        std::vector<int> &_fds;

        template <typename ArgT>
        void operator()(const ArgT &arg)
        {
            _w(arg);
        }

        void operator()(int fd)
        {
            std::println("__AA__ fd=[{}]", fd);
            _fds.push_back(fd);
        }
    };

    std::vector<int> _fd_payload;
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
        auto msg_view = encode(req);
        std::println("-> Sync {}", dump_message(msg_view));
        _s.send(msg_view);
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

    void create_pool()
    {
        pool.emplace(_s, _id_manager);

        wayland::wl_shm::message_create_pool<int> msg;

        size_t sz = 1024 * 1024;
        int memfd = memfd_create("SHM", O_RDWR);
        ftruncate(memfd, sz);
        std::println("__AA__ mem=[{}]", memfd);
        msg.fd = memfd;
        msg.id = pool.value();
        msg.size = wlalat::Int{sz};

        wayland::wl_shm::Request<int> req{msg};
        wlalat::MessageViewFD<int> req_msg = _raw_msg.prepare(*this, req);
        std::println("-> Req message_create_pool {}", dump_message(req_msg));
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
            std::println("-> Req shm {}", dump_message(req_msg));
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
    std::println("-> {}", dump_message(raw_m));
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
        std::println("<- {}", dump_message(msg));

        display.dispatch(msg);
        registry.dispatch(msg);
    }

    if (registry.shm) {
        auto &v = registry.shm.value();
        v.create_pool();
    }

    std::this_thread::sleep_for(std::chrono::seconds{10});
    display.sync();
    std::optional<wlalat::MessageView> message_op = s.recv();
    if (message_op) {
        wlalat::MessageView msg = message_op.value();
        std::println("<- {}", dump_message(msg));
        display.dispatch(msg);
        registry.dispatch(msg);
    }
    std::this_thread::sleep_for(std::chrono::seconds{10});

    message_op = s.recv();
    if (message_op) {
        wlalat::MessageView msg = message_op.value();
        std::println("<- {}", dump_message(msg));
        display.dispatch(msg);
        registry.dispatch(msg);
    } else {
        std::println("NO MSG");
    }

} catch (std::exception &e) {
    std::println(stderr, "std::exception [{}]", e.what());
    return EXIT_FAILURE;
}
