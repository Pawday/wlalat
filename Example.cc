#include "wayland.xml.hh"
#include "xdg-shell.xml.hh"

#include <wlalat/Binary.hh>
#include <wlalat/Error.hh>
#include <wlalat/Message.hh>
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

#include <algorithm>
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
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

constexpr std::chrono::seconds message_timeout{1};

size_t w = 640;
size_t h = 480;

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
        msg.object_id.raw(),
        msg.opcode,
        hexdump(msg.payload));
}

struct ObjectIDManager
{
    struct ID : wlalat::NewID
    {
        operator wlalat::Object() const
        {
            return wlalat::Object{raw()};
        }
    };

    ID allocate()
    {
        auto O = _next_free;
        _next_free = wlalat::NewID{O.raw() + 1};
        return ID{O};
    }

  private:
    wlalat::NewID _next_free{2};
};

static bool operator==(const ObjectIDManager::ID &l, const wlalat::Object &o)
{
    return l.raw() == o.raw();
}

struct MessageOwner
{
    template <typename MsgPayloadT>
    wlalat::MessageViewFD<int>
        prepare(ObjectIDManager::ID object_id, const MsgPayloadT &P)
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
            _fds.push_back(fd);
        }
    };

    std::vector<int> _fd_payload;
    std::vector<std::byte> _payload;
};

template <typename TagT>
struct TagName;
// clang-format off
template <> struct TagName<wayland::wl_shm>         { static constexpr std::string_view value{"wl_shm"};};
template <> struct TagName<wayland::wl_compositor>  { static constexpr std::string_view value{"wl_compositor"};};
template <> struct TagName<xdg_shell::xdg_wm_base>  { static constexpr std::string_view value{"xdg_wm_base"};};
// clang-format on

template <typename TagT>
static constexpr std::string_view TagNameV = TagName<TagT>::value;

struct Display
{
    using Tag = wayland::wl_display;

    Display(wlalat::Unix::Socket &s, ObjectIDManager &id_manager)
        : _s{s}, _id_manager{id_manager}
    {
    }

    wlalat::MessageView encode(wayland::wl_display::Request m)
    {
        return _raw_msg.prepare(_id, m);
    }

    void sync()
    {
        wayland::wl_display::message_sync msg;
        msg.callback = _id_manager.allocate();
        wayland::wl_display::Request req{msg};
        auto msg_view = encode(req);
        std::println("-> Sync {}", dump_message(msg_view));
        _s.send(_id, req);
    }

    void dispatch(wlalat::MessageView M)
    {
        if (M.object_id != _id) {
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
        uint32_t object_id = m.object_id.raw();
        uint32_t code = m.code;
        std::string_view message = m.message;
        std::println(
            "wl_display@1.error(object_id=[{}] code=[{}], message=[{}])",
            object_id,
            code,
            message);
    }

    void on(const wayland::wl_display::message_delete_id &m)
    {
        std::println("wl_display@1.delete_id({:x})", m.id.raw());
    }

  private:
    static constexpr ObjectIDManager::ID _id{1};
    wlalat::Unix::Socket &_s;
    ObjectIDManager &_id_manager;

    MessageOwner _raw_msg;
};

struct Surface
{
    using Tag = wayland::wl_surface;

    Surface(ObjectIDManager::ID id, wlalat::Unix::Socket &s)
        : _id{id}, _s{s} {};

    void attach(ObjectIDManager::ID buffer_id)
    {
        wayland::wl_surface::message_attach msg{};
        msg.buffer = buffer_id;
        std::println(
            "-> wl_surface@{}.attach(buffer=[{}])",
            _id.raw(),
            msg.buffer.raw());
        _s.send(_id, wayland::wl_surface::Request{msg});
    }

    void commit()
    {
        wayland::wl_surface::message_commit msg{};
        std::println("-> wl_surface@{}.commit()", _id.raw());
        _s.send(_id, wayland::wl_surface::Request{msg});
    }

    auto id() const
    {
        return _id;
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
    MessageOwner _raw_msg;
};

struct Compositor
{
    using Tag = wayland::wl_compositor;

    Compositor(ObjectIDManager::ID id, wlalat::Unix::Socket &s)
        : _id{id}, _s{s} {};

    auto create_surface(ObjectIDManager &id_manager)
    {
        auto O = id_manager.allocate();

        wayland::wl_compositor::message_create_surface msg{};
        msg.id = O;

        std::println(
            "-> wl_compositor@{}.message_create_surface(id=[{}])",
            _id.raw(),
            msg.id.raw());
        _s.send(_id, wayland::wl_compositor::Request{msg});
        return O;
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
    MessageOwner _raw_msg;
};

struct Buffer
{
    using Tag = wayland::wl_buffer;

    Buffer(ObjectIDManager::ID id, wlalat::Unix::Socket &s) : _id{id}, _s{s} {};

    void dispatch(wlalat::MessageView &M)
    {
        if (M.object_id != _id) {
            return;
        }

        wlalat::Parser P{M.payload};

        auto ev_op = wayland::wl_buffer::Event::parse(P, M.opcode);
        if (!ev_op) {
            return;
        }

        auto vis = [&]<typename EvT>(const EvT &ev) { on(ev); };
        std::visit(vis, ev_op.value());
    }

    void on(const wayland::wl_buffer::message_release &m)
    {
        released = true;
        std::println("wl_buffer@{}.release()", _id.raw());
    }

    auto id() const
    {
        return _id;
    }

    bool released = true;

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
};

struct ShmPool
{
    using Tag = wayland::wl_shm_pool;

    ShmPool(ObjectIDManager::ID id, wlalat::Unix::Socket &s) : _id{id}, _s{s}
    {
    }

    ObjectIDManager::ID create_buffer(ObjectIDManager &id_manager)
    {
        auto O = id_manager.allocate();
        wayland::wl_shm_pool::message_create_buffer msg{};
        msg.id = O;
        msg.offset = wlalat::Int{0};
        msg.width = wlalat::Int{w};
        msg.height = wlalat::Int{h};
        msg.stride = wlalat::Int{w * 4};
        msg.format = wlalat::UInt{0};
        auto req = _raw_msg.prepare(_id, wayland::wl_shm_pool::Request{msg});
        std::println(
            "-> wl_shm_pool@{}.create_buffer({})",
            _id.raw(),
            dump_message(req));
        _s.send(_id, wayland::wl_shm_pool::Request{msg});
        return O;
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
    MessageOwner _raw_msg;
};

struct Shm
{
    using Tag = wayland::wl_shm;

    Shm(wlalat::Unix::Socket &s, ObjectIDManager::ID id) : _id{id}, _s{s}
    {
    }

    ObjectIDManager::ID create_pool(ObjectIDManager &id_manager)
    {
        ObjectIDManager::ID O = id_manager.allocate();

        wayland::wl_shm::message_create_pool<int> msg;

        size_t sz = w * h * 4;
        int memfd = memfd_create("SHM", O_RDWR);
        ftruncate(memfd, sz);
        msg.fd = memfd;
        msg.id = O;
        msg.size = wlalat::Int{sz};

        auto mem =
            mmap(nullptr, sz, PROT_WRITE | PROT_WRITE, MAP_SHARED, memfd, 0);
        if (mem == MAP_FAILED) {
            auto err = errno;
            throw std::runtime_error{
                std::format("Map failed {}", strerror(err))};
        }

        for (size_t y = 0; y != h; ++y) {
            for (size_t x = 0; x != w; ++x) {
                size_t offset = (y * w + x) * 4;
                uint8_t *pixel_p = static_cast<uint8_t *>(mem);
                pixel_p += offset;
                auto &c0 = pixel_p[0];
                auto &c1 = pixel_p[1];
                auto &c2 = pixel_p[2];
                auto &c3 = pixel_p[3];

                c0 = 0;
                c1 = float(y) / h * 255;
                c2 = float(x) / w * 255;
                c3 = 0xff;
            }
        }

        wayland::wl_shm::Request<int> req{msg};
        wlalat::MessageViewFD<int> req_msg = _raw_msg.prepare(_id, req);
        std::println("-> Req message_create_pool {}", dump_message(req_msg));
        _s.send(_id, req);

        return O;
    }

    void on(wayland::wl_shm::message_format fmt)
    {
        std::println(
            "wl_shm@{}.format(format=[{}])", _id.raw(), fmt.format.raw());
    }

    void dispatch(wlalat::MessageView M)
    {
        if (M.object_id != _id) {
            return;
        }

        wlalat::Parser P{M.payload};

        auto ev_op = wayland::wl_shm::Event::parse(P, M.opcode);
        if (!ev_op) {
            return;
        }

        auto vis = [&]<typename EvT>(const EvT &ev) { on(ev); };
        std::visit(vis, ev_op.value());
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
    MessageOwner _raw_msg;
};

struct XDGTopLevel
{
    using Tag = xdg_shell::xdg_toplevel;
    XDGTopLevel(ObjectIDManager::ID id, wlalat::Unix::Socket &s)
        : _id{id}, _s{s}
    {
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
    MessageOwner _raw_msg;
};

struct XDGSurface
{
    using Tag = xdg_shell::xdg_surface;
    XDGSurface(ObjectIDManager::ID id, wlalat::Unix::Socket &s) : _id{id}, _s{s}
    {
    }

    void dispatch(wlalat::MessageView M)
    {
        if (M.object_id != _id) {
            return;
        }
        wlalat::Parser P{M.payload};
        auto ev_op = xdg_shell::xdg_surface::Event::parse(P, M.opcode);
        if (!ev_op) {
            return;
        }
        auto &ev = ev_op.value();
        auto vis = [&]<typename EvT>(const EvT &ev) { on(ev); };
        std::visit(vis, ev);
    }

    void on(const xdg_shell::xdg_surface::message_configure &M)
    {
        std::println(
            "xdg_surface@{}.configure(serial=[{}])", _id.raw(), M.serial.raw());
        ack_configure(M.serial);
        configured = true;
    }

    void ack_configure(wlalat::UInt serial)
    {
        xdg_shell::xdg_surface::message_ack_configure msg{};
        msg.serial = serial;
        std::println(
            "-> xdg_surface@{}.ack_configure(serial=[{}])",
            _id.raw(),
            msg.serial.raw());
        _s.send(_id, xdg_shell::xdg_surface::Request{msg});
    }

    auto get_top_level(ObjectIDManager &id_manager)
    {
        auto O = id_manager.allocate();

        xdg_shell::xdg_surface::message_get_toplevel msg{};
        msg.id = O;

        std::println(
            "-> xdg_surface@{}.get_top_level(id=[{}])",
            _id.raw(),
            msg.id.raw());
        _s.send(_id, xdg_shell::xdg_surface::Request{msg});
        return O;
    }

    bool configured = false;

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
    MessageOwner _raw_msg;
};

struct XDGBase
{
    using Tag = xdg_shell::xdg_wm_base;

    XDGBase(ObjectIDManager::ID id, wlalat::Unix::Socket &s) : _id{id}, _s{s}
    {
    }

    void dispatch(wlalat::MessageView M)
    {
        if (M.object_id != _id) {
            return;
        }
        wlalat::Parser P{M.payload};
        auto ev_op = xdg_shell::xdg_wm_base::Event::parse(P, M.opcode);
        if (!ev_op) {
            return;
        }
        auto &ev = ev_op.value();
        auto vis = [&]<typename EvT>(const EvT &ev) { on(ev); };
        std::visit(vis, ev);
    }

    void on(const xdg_shell::xdg_wm_base::message_ping &M)
    {
        std::println(
            "xdg_wm_base@{}.ping(serial=[{}])", _id.raw(), M.serial.raw());
        pong(M.serial);
    }

    void pong(wlalat::UInt serial)
    {
        xdg_shell::xdg_wm_base::message_pong msg{};
        msg.serial = serial;
        std::println(
            "-> xdg_wm_base@{}.pong(serial=[{}])", _id.raw(), msg.serial.raw());
        _s.send(_id, xdg_shell::xdg_wm_base::Request{msg});
    }

    ObjectIDManager::ID get_xdg_surface(
        ObjectIDManager &id_manager, ObjectIDManager::ID surface_id)
    {
        auto O = id_manager.allocate();
        xdg_shell::xdg_wm_base::message_get_xdg_surface msg{};
        msg.id = O;
        msg.surface = surface_id;
        std::println(
            "-> xdg_wm_base@{}.get_xdg_surface(id=[{}],surface=[{}])",
            _id.raw(),
            msg.id.raw(),
            msg.surface.raw());
        _s.send(_id, xdg_shell::xdg_wm_base::Request{msg});
        return O;
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
    MessageOwner _raw_msg;
};

struct Registry
{
    using Tag = wayland::wl_registry;

    Registry(
        wlalat::Unix::Socket &s,
        ObjectIDManager::ID id,
        ObjectIDManager &id_manager)
        : _id{id}, _s{s}, _id_manager{id_manager}
    {
    }

    void dispatch(wlalat::MessageView M)
    {
        if (M.object_id != _id) {
            return;
        }
        wlalat::Parser P{M.payload};
        auto ev_op = wayland::wl_registry::Event::parse(P, M.opcode);
        if (!ev_op) {
            return;
        }
        auto &ev = ev_op.value();
        auto vis = [&]<typename EvT>(const EvT &ev) { on(ev); };
        std::visit(vis, ev);
    }

    void on(const wayland::wl_registry::message_global &msg)
    {
        uint32_t name = msg.name;
        std::string_view interface{msg.interface};
        uint32_t version = msg.version;
        std::println(
            "wl_registry@{}.global(name=[{}], interface=[{}], version=[{}])",
            _id.raw(),
            name,
            interface,
            version);

        Global new_global;
        new_global.numeric_name = name;
        new_global.interface = interface;
        new_global.version = version;
        globals.push_back(std::move(new_global));
    }

    template <typename TagT>
    std::optional<ObjectIDManager::ID> try_bind(
        ObjectIDManager &id_manager,
        std::type_identity<TagT> interface,
        std::optional<uint32_t> version)
    {
        std::string_view interface_name = TagNameV<TagT>;

        auto match = [&](const Global &g) {
            if (g.interface != interface_name) {
                return false;
            }

            if (version && version.value() != g.version) {
                return false;
            }

            return true;
        };

        auto global_it = std::ranges::find_if(globals, match);
        if (global_it == std::end(globals)) {
            return {};
        }

        Global &global = *global_it;

        wayland::wl_registry::message_bind bind_msg;
        bind_msg.name = wlalat::UInt{global.numeric_name};
        bind_msg.id_interface_name_amogus_arg = TagNameV<TagT>;
        wlalat::UInt bind_version{global.version};
        if (version) {
            bind_version = wlalat::UInt{version.value()};
        }
        bind_msg.id_interface_version_amogus_arg = bind_version;
        ObjectIDManager::ID new_id = id_manager.allocate();
        bind_msg.id = new_id;
        wayland::wl_registry::Request req{bind_msg};
        std::println(
            "-> wl_registry@{}.bind(name=[{}], "
            "id_interface_name_amogus_arg=[{}], "
            "id_interface_version_amogus_arg=[{}], id=[{}])",
            _id.raw(),
            bind_msg.name.raw(),
            std::string_view{bind_msg.id_interface_name_amogus_arg},
            bind_msg.id_interface_version_amogus_arg.raw(),
            bind_msg.id.raw());
        _s.send(_id, req);
        return new_id;
    }

    void on(const wayland::wl_registry::message_global_remove &msg)
    {
        uint32_t name = msg.name;
        std::println(
            "wl_registry@{}.global_remove(name=[{}])", _id.raw(), name);
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
    MessageOwner _raw_msg;
    ObjectIDManager &_id_manager;

    struct Global
    {
        uint32_t numeric_name;
        std::string interface;
        uint32_t version;
    };

    std::vector<Global> globals;
};

int main()
try {
    ObjectIDManager id_manager;
    wlalat::Unix::Socket s;

    Display display{s, id_manager};

    std::optional<Shm> shm;
    std::optional<ShmPool> pool;
    std::optional<Buffer> buffer;
    std::optional<Compositor> compositor;
    std::optional<Surface> surface;

    bool surface_attached = false;

    std::optional<XDGBase> xdg;
    std::optional<XDGSurface> xdg_surface;
    std::optional<XDGTopLevel> xdg_top_level;

    bool initial_commit = false;

    auto registry_tag = id_manager.allocate();
    Registry registry{s, registry_tag, id_manager};

    wayland::wl_display::message_get_registry m{};
    m.registry = registry_tag;

    wlalat::MessageView raw_m = display.encode({m});
    std::println("-> {}", dump_message(raw_m));
    s.send(wlalat::Object{1}, wayland::wl_display::Request{m});

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
        if (shm) {
            shm->dispatch(msg);
        }

        if (buffer) {
            buffer->dispatch(msg);
        }

        if (xdg) {
            xdg->dispatch(msg);
        }

        if (xdg_surface) {
            xdg_surface->dispatch(msg);
        }

        if (!shm) {
            auto id_op = registry.try_bind(
                id_manager, std::type_identity<Shm::Tag>{}, {});
            if (id_op) {
                shm.emplace(s, id_op.value());
            }
        }

        if (!compositor) {
            auto compositor_id_op = registry.try_bind(
                id_manager, std::type_identity<Compositor::Tag>(), {});
            if (compositor_id_op) {
                compositor.emplace(compositor_id_op.value(), s);
            }
        }

        if (!xdg) {
            auto id_op = registry.try_bind(
                id_manager, std::type_identity<XDGBase::Tag>(), {});
            if (id_op) {
                xdg.emplace(id_op.value(), s);
            }
        }

        if (!xdg_surface && surface && xdg) {
            auto id = xdg->get_xdg_surface(id_manager, surface->id());
            xdg_surface.emplace(id, s);
        }

        if (!xdg_top_level && xdg_surface) {
            auto id = xdg_surface->get_top_level(id_manager);
            xdg_top_level.emplace(id, s);
        }

        if (compositor && !surface) {
            auto id = compositor->create_surface(id_manager);
            surface.emplace(id, s);
        }

        if (shm && !pool) {
            auto pool_id = shm->create_pool(id_manager);
            pool.emplace(pool_id, s);
        }

        if (pool && !buffer) {
            auto buffer_id = pool->create_buffer(id_manager);
            buffer.emplace(buffer_id, s);
        }

        bool can_attach = xdg_surface && xdg_surface->configured;
        can_attach = can_attach && surface && buffer;
        can_attach = can_attach && !surface_attached;
        if (can_attach) {
            surface->attach(buffer->id());
            surface_attached = true;
        }

        bool can_initial_commit = !initial_commit;
        can_initial_commit = can_initial_commit && surface && xdg_surface;
        can_initial_commit = can_initial_commit && !xdg_surface->configured;
        if (can_initial_commit) {
            surface->commit();
            initial_commit = true;
        }

        if (xdg_surface && xdg_surface->configured && buffer->released) {
            surface->commit();
            buffer->released = false;
        }
    }

} catch (std::exception &e) {
    std::println(stderr, "std::exception [{}]", e.what());
    return EXIT_FAILURE;
}
