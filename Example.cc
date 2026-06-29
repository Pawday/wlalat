#include "wayland.xml.hh"
#include "wlalat/Traits.hh"
#include "xdg-shell.xml.hh"

#include <array>
#include <ranges>
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

template <typename MsgT>
struct TypeFormatVis
{
    MsgT &M;

    template <typename Ptr>
    std::string operator()(const Ptr &ptr)
    {
        const auto &member = M.*(ptr);
        return my_format(member);
    }

    std::string operator()(std::monostate)
    {
        return "";
    }

  private:
    std::string my_format(const wlalat::NewID &v)
    {
        return std::format("(new_id){}", v.raw());
    }

    std::string my_format(const wlalat::Object &v)
    {
        return std::format("(object){}", v.raw());
    }

    std::string my_format(const wlalat::Int &v)
    {
        return std::format("(int){}", v.raw());
    }

    std::string my_format(const wlalat::UInt &v)
    {
        return std::format("(uint){}]", v.raw());
    }

    std::string my_format(const int &v)
    {
        return std::format("(fd){}", v);
    }

    std::string my_format(const wlalat::String &v)
    {
        return std::format("\"{}\"", std::string_view{v});
    }
};

template <typename MsgT>
std::string dump_message_args(const MsgT &M)
{
    auto member_ptrs = wlalat::Traits<MsgT>::arg_member_pointers;
    auto names = wlalat::Traits<MsgT>::arg_names;
    auto pairs = std::views::zip(member_ptrs, names);

    std::string O;

    bool f = true;
    for (const auto &[ptr, name] : pairs) {
        if (!f) {
            O += ", ";
        }
        f = false;
        O += std::format("{}={}", name, std::visit(TypeFormatVis{M}, ptr));
    }

    return O;
}

template <typename EventT, typename VisitorT>
void iterate_events_on(std::type_identity<EventT>, VisitorT &V)
{
    static constexpr auto event_message_type_idx =
        []<typename... Events>(std::type_identity<std::variant<Events...>>) {
            using TypeIdentityVariantT =
                std::variant<std::type_identity<Events>...>;
            return std::array<
                TypeIdentityVariantT,
                std::variant_size_v<TypeIdentityVariantT>>{
                std::type_identity<Events>{}...};
        }(std::type_identity<EventT>{});

    for (auto &type : event_message_type_idx) {
        std::visit(V, type);
    }
}

template <typename MessageT>
struct ArgParseVisitor
{
    MessageT &M;
    wlalat::Parser &P;

    template <typename ArgMemberPointerT>
    bool operator()(ArgMemberPointerT ptr)
    {
        return P(M.*ptr);
    }

    bool operator()(std::monostate)
    {
        return true;
    }
};

template <typename OEventT>
struct OpcodeDispatchVisitor
{
    uint_least16_t &opcode;
    wlalat::Parser &P;
    std::optional<OEventT> &o_event;

    template <typename MessageT>
    void operator()(std::type_identity<MessageT> m_type_id)
    {
        if (o_event) {
            return;
        }
        if (opcode != wlalat::Traits<MessageT>::opcode) {
            return;
        }

        bool good = true;
        MessageT M{};
        ArgParseVisitor arg_parser{M, P};
        for (auto m_ptr : wlalat::Traits<MessageT>::arg_member_pointers) {
            good = good && std::visit(arg_parser, m_ptr);
        }

        if (!good) {
            return;
        }

        o_event = std::move(M);
    }
};

template <typename InterfaceT>
auto parse_event(std::type_identity<InterfaceT>, wlalat::MessageView M)
{
    using EventT = typename wlalat::Traits<InterfaceT>::Event;
    std::optional<EventT> O;
    wlalat::Parser P{M.payload};
    OpcodeDispatchVisitor V{M.opcode, P, O};
    iterate_events_on(std::type_identity<EventT>{}, V);
    return O;
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

    void sync()
    {
        wayland::wl_display::message_sync msg;
        msg.callback = _id_manager.allocate();
        std::println("-> Sync {}", dump_message_args(msg));
        _s.send(_id, msg);
    }

    void dispatch(wlalat::MessageView M)
    {
        if (M.object_id != _id) {
            return;
        }

        auto ev_op = parse_event(std::type_identity<wayland::wl_display>{}, M);
        if (!ev_op) {
            return;
        }

        auto vis = [&]<typename EvT>(const EvT &ev) { on(ev); };
        std::visit(vis, ev_op.value());
    }

    void on(const wayland::wl_display::message_error &m)
    {
        std::println("<- wl_display@1.error({})", dump_message_args(m));
    }

    void on(const wayland::wl_display::message_delete_id &m)
    {
        std::println("<- wl_display@1.delete_id({})", dump_message_args(m));
    }

  private:
    static constexpr ObjectIDManager::ID _id{1};
    wlalat::Unix::Socket &_s;
    ObjectIDManager &_id_manager;
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
            "-> wl_surface@{}.attach({})", _id.raw(), dump_message_args(msg));
        _s.send(_id, msg);
    }

    void commit()
    {
        wayland::wl_surface::message_commit msg{};
        std::println(
            "-> wl_surface@{}.commit({})", _id.raw(), dump_message_args(msg));
        _s.send(_id, msg);
    }

    auto id() const
    {
        return _id;
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
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
            "-> wl_compositor@{}.message_create_surface({})",
            _id.raw(),
            dump_message_args(msg));
        _s.send(_id, msg);
        return O;
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
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

        auto ev_op = parse_event(std::type_identity<wayland::wl_buffer>{}, M);
        if (!ev_op) {
            return;
        }

        auto vis = [&]<typename EvT>(const EvT &ev) { on(ev); };
        std::visit(vis, ev_op.value());
    }

    void on(const wayland::wl_buffer::message_release &m)
    {
        released = true;
        std::println(
            "<- wl_buffer@{}.release({})", _id.raw(), dump_message_args(m));
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
        std::println(
            "-> wl_shm_pool@{}.create_buffer({})",
            _id.raw(),
            dump_message_args(msg));
        _s.send(_id, msg);
        return O;
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
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

        std::println(
            "-> wl_shm@{}.create_pool({})", _id.raw(), dump_message_args(msg));
        _s.send(_id, msg);

        return O;
    }

    void on(wayland::wl_shm::message_format fmt)
    {
        std::println(
            "<- wl_shm@{}.format({})", _id.raw(), dump_message_args(fmt));
    }

    void dispatch(wlalat::MessageView M)
    {
        if (M.object_id != _id) {
            return;
        }

        auto ev_op = parse_event(std::type_identity<wayland::wl_shm>{}, M);
        if (!ev_op) {
            return;
        }

        auto vis = [&]<typename EvT>(const EvT &ev) { on(ev); };
        std::visit(vis, ev_op.value());
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
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
        auto ev_op =
            parse_event(std::type_identity<xdg_shell::xdg_surface>{}, M);
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
            "<- xdg_surface@{}.configure({})", _id.raw(), dump_message_args(M));
        ack_configure(M.serial);
        configured = true;
    }

    void ack_configure(wlalat::UInt serial)
    {
        xdg_shell::xdg_surface::message_ack_configure msg{};
        msg.serial = serial;
        std::println(
            "-> xdg_surface@{}.ack_configure({})",
            _id.raw(),
            dump_message_args(msg));
        _s.send(_id, msg);
    }

    auto get_top_level(ObjectIDManager &id_manager)
    {
        auto O = id_manager.allocate();

        xdg_shell::xdg_surface::message_get_toplevel msg{};
        msg.id = O;

        std::println(
            "-> xdg_surface@{}.get_top_level({})",
            _id.raw(),
            dump_message_args(msg));
        _s.send(_id, msg);
        return O;
    }

    bool configured = false;

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
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
        auto ev_op =
            parse_event(std::type_identity<xdg_shell::xdg_wm_base>{}, M);
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
            "<- xdg_wm_base@{}.ping({})", _id.raw(), dump_message_args(M));
        pong(M.serial);
    }

    void pong(wlalat::UInt serial)
    {
        xdg_shell::xdg_wm_base::message_pong msg{};
        msg.serial = serial;
        std::println(
            "-> xdg_wm_base@{}.pong({})", _id.raw(), dump_message_args(msg));
        _s.send(_id, msg);
    }

    ObjectIDManager::ID get_xdg_surface(
        ObjectIDManager &id_manager, ObjectIDManager::ID surface_id)
    {
        auto O = id_manager.allocate();
        xdg_shell::xdg_wm_base::message_get_xdg_surface msg{};
        msg.id = O;
        msg.surface = surface_id;
        std::println(
            "-> xdg_wm_base@{}.get_xdg_surface({})",
            _id.raw(),
            dump_message_args(msg));
        _s.send(_id, msg);
        return O;
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
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
        auto ev_op = parse_event(std::type_identity<wayland::wl_registry>{}, M);
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
            "<- wl_registry@{}.global({})", _id.raw(), dump_message_args(msg));

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
        std::println(
            "-> wl_registry@{}.bind({})",
            _id.raw(),
            dump_message_args(bind_msg));
        _s.send(_id, bind_msg);
        return new_id;
    }

    void on(const wayland::wl_registry::message_global_remove &msg)
    {
        uint32_t name = msg.name;
        std::println(
            "<- wl_registry@{}.global_remove({})",
            _id.raw(),
            dump_message_args(msg));
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
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

    std::println(
        "-> wl_display@1.message_get_registry({})", dump_message_args(m));
    s.send(wlalat::Object{1}, m);

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
        std::println(
            "<- @{}.#{}({})",
            msg.object_id.raw(),
            msg.opcode,
            hexdump(msg.payload));

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
