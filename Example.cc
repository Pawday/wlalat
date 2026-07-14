#include "wayland.xml.hh"
#include "xdg-decoration-unstable-v1.xml.hh"
#include "xdg-shell.xml.hh"

#include <wlalat/Binary.hh>
#include <wlalat/Error.hh>
#include <wlalat/MessageHeader.hh>
#include <wlalat/MessageSerializer.hh>
#include <wlalat/Parser.hh>
#include <wlalat/StringParser.hh>
#include <wlalat/Traits.hh>
#include <wlalat/Types.hh>
#include <wlalat/Unix/Socket.hh>
#include <wlalat/Unix/TypeTags.hh>
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
#include <array>
#include <chrono>
#include <exception>
#include <format>
#include <iterator>
#include <memory>
#include <optional>
#include <print>
#include <queue>
#include <ranges>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
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

template <typename TupleT>
struct Tuple2Variant;
template <typename... Alternatives>
struct Tuple2Variant<std::tuple<Alternatives...>>
    : std::type_identity<std::variant<Alternatives...>>
{
};

template <>
struct Tuple2Variant<std::tuple<>>
    : std::type_identity<std::variant<std::monostate>>
{
};

template <typename TupleT>
using Tuple2VariantT = typename Tuple2Variant<TupleT>::type;

template <typename MsgT>
struct TypeFormatVis
{
    using MsgMeta =
        decltype(MsgT::Meta::template args_meta<wlalat::Unix::WlTags>);
    using Indexes = std::make_index_sequence<std::tuple_size_v<MsgMeta>>;

    TypeFormatVis(MsgT &M) : M{M}
    {
    }

    std::string format()
    {
        _output.clear();
        MyFormatDispatcher<>{*this}.format();
        return _output;
    }

  private:
    using WlTags = wlalat::Unix::WlTags;

    template <typename SEQ = Indexes>
    struct MyFormatDispatcher;

    template <size_t... FIDXS>
    struct MyFormatDispatcher<std::index_sequence<FIDXS...>>
    {
        TypeFormatVis &V;

        void format()
        {
            size_t max = std::index_sequence<FIDXS...>::size();
            for (size_t idx = 0; idx != max; ++idx) {
                (rt(std::integral_constant<size_t, FIDXS>{}, idx), ...);
            }
        }

      private:
        template <size_t IDX>
        void rt(std::integral_constant<size_t, IDX>, size_t rt_idx)
        {
            if (IDX != rt_idx) {
                return;
            }

            constexpr auto &args_meta = MsgT::Meta::template args_meta<WlTags>;
            constexpr auto &arg_meta = std::get<IDX>(args_meta);

            constexpr auto &tag = std::get<0>(arg_meta);
            constexpr auto &ptr = std::get<1>(arg_meta);
            constexpr auto &name = std::get<2>(arg_meta);
            V.my_format_taged(rt_idx, tag, V.M.*ptr, name);
        }
    };

    template <typename TagT, typename ValueT>
    void my_format_taged(
        size_t idx, TagT tag, const ValueT &v, std::string_view name)
    {
        if (idx != 0) {
            std::format_to(O, ", ");
        }
        std::format_to(O, "{}=", name);
        my_format(tag, v);
    }

    void my_format(WlTags::wl_new_id, const uint_least32_t &v)
    {
        std::format_to(O, "(new_id)@{}", v);
    }

    void my_format(WlTags::wl_object, const uint_least32_t &v)
    {
        std::format_to(O, "(object)@{}", v);
    }

    void my_format(WlTags::wl_int, const int_least32_t &v)
    {
        std::format_to(O, "(int){}", v);
    }

    void my_format(WlTags::wl_array, const std::span<const std::byte> &v)
    {
        std::format_to(O, "(array){}", hexdump(v));
    }

    void my_format(WlTags::wl_uint, const uint_least32_t &v)
    {
        std::format_to(O, "(uint){}", v);
    }

    void my_format(WlTags::wl_fd, const void *vp)
    {
        const int *vip = static_cast<const int *>(vp);
        std::format_to(O, "(fd){}", *vip);
    }

    void my_format(WlTags::wl_string, std::string_view v)
    {
        std::format_to(O, "\"{}\"", std::string_view{v});
    }

    const MsgT &M;
    std::string _output{};
    std::back_insert_iterator<std::string> O = std::back_inserter(_output);
};

template <typename MsgT>
std::string dump_message_args(const MsgT &M)
{
    return TypeFormatVis{M}.format();
}

template <typename MsgT>
std::string dump_message(const MsgT &M)
{
    return std::format("{}({})", MsgT::Meta::name, dump_message_args(M));
}

template <typename... Alternatives>
auto alternatives_array(std::type_identity<std::variant<Alternatives...>>)
{
    using OutElT = std::variant<std::type_identity<Alternatives>...>;
    return std::array<OutElT, std::variant_size_v<OutElT>>{
        std::type_identity<Alternatives>{}...};
}

struct ObjectIDManager
{
    struct ID : wlalat::NewID
    {
        operator wlalat::Object() const
        {
            return wlalat::Object{raw()};
        }

        operator uint_least32_t() const
        {
            return raw();
        }
    };

    ID allocate()
    {
        if (!_avail.empty()) {
            auto O = _avail.front();
            _allocated.emplace(O);
            _avail.pop();
            return ID{O};
        }
        auto O = _next_free;
        _allocated.emplace(O.raw());
        _next_free = wlalat::NewID{O.raw() + 1};
        return ID{O};
    }

    void deallocate(ID id)
    {
        auto allocated_it = _allocated.find(id.raw());
        if (std::end(_allocated) == allocated_it) {
            auto msg = std::format(
                "ObjectIDManager::deallocate({}): ID not allocated", id.raw());
            throw std::runtime_error{std::move(msg)};
        }
        _avail.push(id.raw());
        _allocated.erase(allocated_it);
    }

  private:
    wlalat::NewID _next_free{2};
    std::unordered_set<uint_least32_t> _allocated;
    std::queue<uint_least32_t> _avail;
};

static bool operator==(const ObjectIDManager::ID &l, const wlalat::Object &o)
{
    return l.raw() == o.raw();
}

struct SocketEventDispatcher
{
    SocketEventDispatcher(wlalat::Unix::Socket &s) : _s{s}
    {
    }

    template <typename LType, typename InterfaceT>
    void set_listener(
        uint_least32_t object_id, LType &L, std::type_identity<InterfaceT>)
    {
        using VariantT =
            Tuple2VariantT<typename wlalat::Traits<InterfaceT>::Events>;
        std::type_identity<VariantT> event_id{};
        _listeners.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(object_id),
            std::forward_as_tuple(_s, std::addressof(L), event_id));
    }

    void remove_listener(uint_least32_t object_id)
    {
        auto listener_it = _listeners.find(object_id);
        if (listener_it == std::end(_listeners)) {
            auto msg = std::format(
                "SocketEventDispatcher::remove_listener({}): Listener does not "
                "exist",
                object_id);
            throw std::runtime_error{std::move(msg)};
        }
        _listeners.erase(listener_it);
    }

    bool recv_dispatch()
    {
        auto H_op = _s.peek_header();
        if (!H_op) {
            return false;
        }
        auto &H = H_op.value();

        auto listener_it = _listeners.find(H.object_id);
        if (listener_it == std::end(_listeners)) {
            auto msg =
                std::format("Missing listener for object@{}", H.object_id);
            std::println("[WARN]: {}", msg);
            return false;
        }
        return listener_it->second.recv_dispatch(H.opcode);
    }

  private:
    struct Listener
    {
        template <typename LType, typename EventVarintT>
        Listener(
            wlalat::Unix::Socket &s, LType *l, std::type_identity<EventVarintT>)
            : _s{s}, _l_ptr{l},
              V_recv_dispatch{&Listener::T_recv_dispatch<EventVarintT, LType>}
        {
        }

        bool recv_dispatch(uint_least16_t opcode)
        {
            return (this->*V_recv_dispatch)(opcode);
        }

        template <typename LType>
        struct T_recv_dispatch_visitor
        {
            LType &L;
            uint_least16_t &opcode;
            wlalat::Unix::Socket &s;

            template <typename MessageT>
            bool operator()(const std::type_identity<MessageT> M)
            {
                if (opcode != MessageT::Meta::opcode) {
                    return false;
                }
                auto m_op = s.recv_event(M);
                if (!m_op) {
                    throw std::runtime_error{m_op.error()};
                }
                L.on(m_op.value());
                return true;
            }

            bool operator()(std::type_identity<std::monostate>)
            {
                return false;
            }
        };

        template <typename EventVarintT, typename LType>
        bool T_recv_dispatch(uint_least16_t opcode)
        {
            LType &L = *static_cast<LType *>(_l_ptr);

            T_recv_dispatch_visitor V{L, opcode, _s};

            for (auto msg_type_id :
                 alternatives_array(std::type_identity<EventVarintT>{})) {
                if (std::visit(V, msg_type_id)) {
                    return true;
                }
            }

            return false;
        }

        wlalat::Unix::Socket &_s;
        void *_l_ptr;
        bool (Listener::*const V_recv_dispatch)(uint_least16_t opcode);
    };

    wlalat::Unix::Socket &_s;
    std::unordered_map<uint_least32_t, Listener> _listeners;
};

struct Callback
{
    using Tag = wayland::wl_callback;

    Callback(ObjectIDManager::ID id, std::optional<Tag::message_done> &done_msg)
        : _id{id}, _done_msg{done_msg}
    {
    }

    void on(const Tag::message_done &M)
    {
        std::println("<- wl_callback@{}.{}", _id.raw(), dump_message(M));
        _done_msg = M;
    }

  private:
    ObjectIDManager::ID _id;
    std::optional<Tag::message_done> &_done_msg;
};

struct Display
{
    using Tag = wayland::wl_display;

    Display(
        wlalat::Unix::Socket &s,
        ObjectIDManager &id_manager,
        SocketEventDispatcher &disp)
        : _s{s}, _id_manager{id_manager}, _disp{disp}
    {
    }

    [[nodiscard]] auto
        sync(std::optional<wayland::wl_callback::message_done> &done_msg)
    {
        wayland::wl_display::message_sync msg;
        msg.callback = _id_manager.allocate();

        auto obj = std::make_shared<Callback>(
            ObjectIDManager::ID{msg.callback}, done_msg);
        _disp.set_listener(
            msg.callback, *obj, std::type_identity<Callback::Tag>{});
        _sync_callbacks[msg.callback] = obj;

        std::println("-> wl_display@1.{}", dump_message(msg));
        _s.send(_id, msg);
        return std::weak_ptr{obj};
    }

    void on(const wayland::wl_display::message_error &m)
    {
        std::println("<- wl_display@1.{}", dump_message(m));
    }

    void on(const wayland::wl_display::message_delete_id &m)
    {
        std::println("<- wl_display@1.{}", dump_message(m));
        _disp.remove_listener(m.id);
        _id_manager.deallocate(ObjectIDManager::ID{m.id});

        auto sync_obj_it = _sync_callbacks.find(m.id);
        if (sync_obj_it != std::end(_sync_callbacks)) {
            _sync_callbacks.erase(sync_obj_it);
        }
    }

  private:
    static constexpr ObjectIDManager::ID _id{1};
    wlalat::Unix::Socket &_s;
    ObjectIDManager &_id_manager;
    SocketEventDispatcher &_disp;

    std::unordered_map<uint_least16_t, std::shared_ptr<Callback>>
        _sync_callbacks;
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
        std::println("-> wl_surface@{}.{}", _id.raw(), dump_message(msg));
        _s.send(_id, msg);
    }

    void commit()
    {
        wayland::wl_surface::message_commit msg{};
        std::println("-> wl_surface@{}.{}", _id.raw(), dump_message(msg));
        _s.send(_id, msg);
    }

    void damage()
    {
        wayland::wl_surface::message_damage msg{};
        msg.x = wlalat::Int{0};
        msg.y = wlalat::Int{0};
        msg.width = wlalat::Int{w};
        msg.height = wlalat::Int{h};
        std::println("-> wl_surface@{}.{}", _id.raw(), dump_message(msg));
        _s.send(_id, msg);
    }

    auto id() const
    {
        return _id;
    }

    template <typename EventT>
    void on(const EventT &ev)
    {
        std::println("<- wl_surface@{}.{}", _id.raw(), dump_message(ev));
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

        std::println("-> wl_compositor@{}.{}", _id.raw(), dump_message(msg));
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

    void on(const wayland::wl_buffer::message_release &m)
    {
        released = true;
        std::println("<- wl_buffer@{}.{}", _id.raw(), dump_message(m));
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

    ObjectIDManager::ID create_buffer(ObjectIDManager &id_manager, size_t id)
    {
        auto O = id_manager.allocate();
        wayland::wl_shm_pool::message_create_buffer msg{};
        msg.id = O;
        msg.offset = wlalat::Int{(w * h * 4) * id};
        msg.width = wlalat::Int{w};
        msg.height = wlalat::Int{h};
        msg.stride = wlalat::Int{w * 4};
        msg.format = wlalat::UInt{0};
        std::println("-> wl_shm_pool@{}.{}", _id.raw(), dump_message(msg));
        _s.send(_id, msg);
        return O;
    }

    template <typename EventT>
    void on(const EventT &ev)
    {
        std::println("<- wl_shm_pool@{}.{}", _id.raw(), dump_message(ev));
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

        wayland::wl_shm::message_create_pool msg;

        size_t sz = (w * h * 4) * 2;
        _fd = memfd_create("SHM", O_RDWR);
        ftruncate(_fd.value(), sz);
        msg.fd = std::addressof(_fd.value());
        msg.id = O;
        msg.size = wlalat::Int{sz};

        auto mem = mmap(
            nullptr, sz, PROT_WRITE | PROT_WRITE, MAP_SHARED, _fd.value(), 0);
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

        for (size_t y = 0; y != h; ++y) {
            for (size_t x = 0; x != w; ++x) {
                size_t offset = (y * w + x) * 4;
                offset += (w * h) * 4;
                uint8_t *pixel_p = static_cast<uint8_t *>(mem);
                pixel_p += offset;
                auto &c0 = pixel_p[0];
                auto &c1 = pixel_p[1];
                auto &c2 = pixel_p[2];
                auto &c3 = pixel_p[3];

                c0 = float(y) / h * 255;
                c1 = float(x) / w * 255;
                c2 = 0;
                c3 = 0xff;
            }
        }

        std::println("-> wl_shm@{}.{}", _id.raw(), dump_message(msg));
        _s.send(_id, msg);

        return O;
    }

    void on(wayland::wl_shm::message_format fmt)
    {
        std::println("<- wl_shm@{}.{}", _id.raw(), dump_message(fmt));
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
    std::optional<int> _fd;
};

struct XDGTopLevel
{
    using Tag = xdg_shell::xdg_toplevel;
    XDGTopLevel(ObjectIDManager::ID id, wlalat::Unix::Socket &s)
        : _id{id}, _s{s}
    {
    }

    template <typename EventT>
    void on(const EventT &ev)
    {
        std::println("<- xdg_toplevel@{}.{}", _id.raw(), dump_message(ev));
    }

    void on(const xdg_shell::xdg_toplevel::message_close &ev)
    {
        std::println("<- xdg_toplevel@{}.{}", _id.raw(), dump_message(ev));
        _close_flag = true;
    }

    auto id() const
    {
        return _id;
    }

    auto close_flag() const
    {
        return _close_flag;
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
    bool _close_flag = false;
};

struct XDGSurface
{
    using Tag = xdg_shell::xdg_surface;
    XDGSurface(ObjectIDManager::ID id, wlalat::Unix::Socket &s) : _id{id}, _s{s}
    {
    }

    void on(const xdg_shell::xdg_surface::message_configure &M)
    {
        std::println("<- xdg_surface@{}.{}", _id.raw(), dump_message(M));
        ack_configure(M.serial);
        configured = true;
    }

    void ack_configure(std::uint_least32_t serial)
    {
        xdg_shell::xdg_surface::message_ack_configure msg{};
        msg.serial = serial;
        std::println("-> xdg_surface@{}.{}", _id.raw(), dump_message(msg));
        _s.send(_id, msg);
    }

    auto get_top_level(ObjectIDManager &id_manager)
    {
        auto O = id_manager.allocate();

        xdg_shell::xdg_surface::message_get_toplevel msg{};
        msg.id = O;

        std::println("-> xdg_surface@{}.{}", _id.raw(), dump_message(msg));
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

    void on(const xdg_shell::xdg_wm_base::message_ping &M)
    {
        std::println("<- xdg_wm_base@{}.{}", _id.raw(), dump_message(M));
        pong(M.serial);
    }

    void pong(std::uint_least32_t serial)
    {
        xdg_shell::xdg_wm_base::message_pong msg{};
        msg.serial = serial;
        std::println("-> xdg_wm_base@{}.{}", _id.raw(), dump_message(msg));
        _s.send(_id, msg);
    }

    ObjectIDManager::ID get_xdg_surface(
        ObjectIDManager &id_manager, ObjectIDManager::ID surface_id)
    {
        auto O = id_manager.allocate();
        xdg_shell::xdg_wm_base::message_get_xdg_surface msg{};
        msg.id = O;
        msg.surface = surface_id;
        std::println("-> xdg_wm_base@{}.{}", _id.raw(), dump_message(msg));
        _s.send(_id, msg);
        return O;
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
};

struct XDGDecorTopLevel
{
    using Tag = xdg_decoration_unstable_v1::zxdg_toplevel_decoration_v1;

    XDGDecorTopLevel(ObjectIDManager::ID id, wlalat::Unix::Socket &s)
        : _id{id}, _s{s}
    {
    }

    void on(const Tag::message_configure &M)
    {
        std::println(
            "<- zxdg_toplevel_decoration_v1@{}.{}", _id.raw(), dump_message(M));
    }

    void set_server_side()
    {
        Tag::message_set_mode M{};
        M.mode = wlalat::UInt{2}; // server side mode
        std::println(
            "-> zxdg_toplevel_decoration_v1@{}.{}", _id.raw(), dump_message(M));
        _s.send(_id, M);
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
};

struct XDGDecorManager
{
    using Tag = xdg_decoration_unstable_v1::zxdg_decoration_manager_v1;
    XDGDecorManager(
        ObjectIDManager::ID id,
        wlalat::Unix::Socket &s,
        ObjectIDManager &id_manager,
        SocketEventDispatcher &disp)
        : _id{id}, _s{s}, _id_manager{id_manager}, _disp{disp}
    {
    }

    void get_toplevel_decoration_to(
        std::optional<XDGDecorTopLevel> &O, const XDGTopLevel &top_level)
    {
        if (O) {
            return;
        }

        Tag::message_get_toplevel_decoration msg{};
        msg.id = _id_manager.allocate();
        msg.toplevel = top_level.id();
        std::println(
            "-> zxdg_decoration_manager_v1@{}.{}",
            _id.raw(),
            dump_message(msg));
        _s.send(_id, msg);
        O.emplace(ObjectIDManager::ID{msg.id}, _s);
        _disp.set_listener(
            msg.id,
            O.value(),
            std::type_identity<
                xdg_decoration_unstable_v1::zxdg_toplevel_decoration_v1>{});
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
    ObjectIDManager &_id_manager;
    SocketEventDispatcher &_disp;
};

struct Registry
{
    using Tag = wayland::wl_registry;

    Registry(
        wlalat::Unix::Socket &s,
        ObjectIDManager::ID id,
        ObjectIDManager &id_manager,
        SocketEventDispatcher &disp)
        : _id{id}, _s{s}, _id_manager{id_manager}, _disp{disp}
    {
        _disp.set_listener(_id.raw(), *this, std::type_identity<Tag>{});
    }

    void on(const wayland::wl_registry::message_global &msg)
    {
        uint32_t name = msg.name;
        std::string_view interface{msg.interface};
        uint32_t version = msg.version;
        std::println("<- wl_registry@{}.{}", _id.raw(), dump_message(msg));

        Global new_global;
        new_global.numeric_name = name;
        new_global.interface = interface;
        new_global.version = version;
        globals.push_back(std::move(new_global));
    }

    std::optional<ObjectIDManager::ID> try_bind(
        std::string_view interface_name, std::optional<uint32_t> version)
    {
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
        bind_msg.id_interface_name_amogus_arg = interface_name;
        wlalat::UInt bind_version{global.version};
        if (version) {
            bind_version = wlalat::UInt{version.value()};
        }
        bind_msg.id_interface_version_amogus_arg = bind_version;
        ObjectIDManager::ID new_id = _id_manager.allocate();
        bind_msg.id = new_id;
        std::println("-> wl_registry@{}.{}", _id.raw(), dump_message(bind_msg));
        _s.send(_id, bind_msg);
        return new_id;
    }

    void on(const wayland::wl_registry::message_global_remove &msg)
    {
        std::println("<- wl_registry@{}.{}", _id.raw(), dump_message(msg));
    }

  private:
    ObjectIDManager::ID _id;
    wlalat::Unix::Socket &_s;
    ObjectIDManager &_id_manager;
    SocketEventDispatcher &_disp;

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

    SocketEventDispatcher disp{s};
    Display display{s, id_manager, disp};

    disp.set_listener(1, display, std::type_identity<wayland::wl_display>{});

    std::optional<Shm> shm;
    std::optional<ShmPool> pool;
    std::optional<Buffer> buffer_0;
    std::optional<Buffer> buffer_1;
    size_t next_buffer_to_use = 0;
    std::optional<Compositor> compositor;
    std::optional<Surface> surface;
    std::optional<XDGDecorManager> decor_manager;

    std::optional<XDGBase> xdg;
    std::optional<XDGSurface> xdg_surface;
    std::optional<XDGTopLevel> xdg_top_level;
    std::optional<XDGDecorTopLevel> xdg_top_level_decor;

    bool initial_commit = false;
    bool top_level_decore_set = false;

    std::chrono::steady_clock::time_point last_buffer_commit_time{};

    auto registry_tag = id_manager.allocate();
    Registry registry{s, registry_tag, id_manager, disp};

    wayland::wl_display::message_get_registry m{};
    m.registry = registry_tag;

    std::println("-> wl_display@1.{}", dump_message(m));
    s.send(wlalat::Object{1}, m);

    auto last_message = std::chrono::steady_clock::now();

    while (true) {
        if (xdg_top_level && xdg_top_level->close_flag()) {
            break;
        }

        auto elapsed = decltype(last_message)::clock::now() - last_message;
        if (elapsed > message_timeout) {
            break;
        }
        auto disp_status = disp.recv_dispatch();
        if (!disp_status) {
            std::this_thread::yield();
        } else {
            last_message = decltype(last_message)::clock::now();
        }

        if (!decor_manager) {
            using Tag = decltype(decor_manager)::value_type::Tag;
            using IdT = std::type_identity<Tag>;
            auto id_op = registry.try_bind(wlalat::Traits<Tag>::name, {});
            if (id_op) {
                decor_manager.emplace(id_op.value(), s, id_manager, disp);
                disp.set_listener(
                    id_op.value().raw(), decor_manager.value(), IdT{});
            }
        }

        if (!xdg_top_level_decor && decor_manager && xdg_top_level) {
            decor_manager->get_toplevel_decoration_to(
                xdg_top_level_decor, xdg_top_level.value());
        }

        if (!top_level_decore_set && xdg_top_level_decor) {
            xdg_top_level_decor->set_server_side();
            top_level_decore_set = true;
        }

        if (!shm) {
            using Tag = decltype(shm)::value_type::Tag;
            using IdT = std::type_identity<Tag>;
            auto id_op = registry.try_bind(wlalat::Traits<Tag>::name, {});
            if (id_op) {
                shm.emplace(s, id_op.value());
                disp.set_listener(id_op.value().raw(), shm.value(), IdT{});
            }
        }

        if (!compositor) {
            using Tag = decltype(compositor)::value_type::Tag;
            using IdT = std::type_identity<Tag>;
            auto id_op = registry.try_bind(wlalat::Traits<Tag>::name, {});
            if (id_op) {
                compositor.emplace(id_op.value(), s);
                disp.set_listener(
                    id_op.value().raw(), compositor.value(), IdT{});
            }
        }

        if (!xdg) {
            using Tag = decltype(xdg)::value_type::Tag;
            using IdT = std::type_identity<Tag>;
            auto id_op = registry.try_bind(wlalat::Traits<Tag>::name, {});
            if (id_op) {
                xdg.emplace(id_op.value(), s);
                using IdT = std::type_identity<decltype(xdg)::value_type::Tag>;
                disp.set_listener(id_op.value().raw(), xdg.value(), IdT{});
            }
        }

        if (!xdg_surface && surface && xdg) {
            auto id = xdg->get_xdg_surface(id_manager, surface->id());
            xdg_surface.emplace(id, s);
            using IdT =
                std::type_identity<decltype(xdg_surface)::value_type::Tag>;
            disp.set_listener(id.raw(), xdg_surface.value(), IdT{});
        }

        if (!xdg_top_level && xdg_surface) {
            auto id = xdg_surface->get_top_level(id_manager);
            xdg_top_level.emplace(id, s);
            using IdT =
                std::type_identity<decltype(xdg_top_level)::value_type::Tag>;
            disp.set_listener(id.raw(), xdg_top_level.value(), IdT{});
        }

        if (compositor && !surface) {
            auto id = compositor->create_surface(id_manager);
            surface.emplace(id, s);
            using IdT = std::type_identity<decltype(surface)::value_type::Tag>;
            disp.set_listener(id.raw(), surface.value(), IdT{});
        }

        if (shm && !pool) {
            auto pool_id = shm->create_pool(id_manager);
            pool.emplace(pool_id, s);
            using IdT = std::type_identity<decltype(pool)::value_type::Tag>;
            disp.set_listener(pool_id.raw(), pool.value(), IdT{});
        }

        if (pool && !buffer_0) {
            auto buffer_id = pool->create_buffer(id_manager, 0);
            buffer_0.emplace(buffer_id, s);
            using IdT = std::type_identity<decltype(buffer_0)::value_type::Tag>;
            disp.set_listener(buffer_id.raw(), buffer_0.value(), IdT{});
        }

        if (pool && !buffer_1) {
            auto buffer_id = pool->create_buffer(id_manager, 1);
            buffer_1.emplace(buffer_id, s);
            using IdT = std::type_identity<decltype(buffer_1)::value_type::Tag>;
            disp.set_listener(buffer_id.raw(), buffer_1.value(), IdT{});
        }

        bool can_initial_commit = !initial_commit;
        can_initial_commit = can_initial_commit && surface && xdg_surface;
        can_initial_commit = can_initial_commit && !xdg_surface->configured;
        if (can_initial_commit) {
            surface->commit();
            initial_commit = true;
        }

        if (xdg_surface && xdg_surface->configured && surface) {
            auto last_buffer_commit_time_diff =
                std::chrono::steady_clock::now() - last_buffer_commit_time;

            Buffer *active_buffer = nullptr;

            if (buffer_0 && buffer_0->released && next_buffer_to_use == 0) {
                active_buffer = &buffer_0.value();
            }
            if (buffer_1 && buffer_1->released && next_buffer_to_use == 1) {
                active_buffer = &buffer_1.value();
            }

            if (active_buffer &&
                last_buffer_commit_time_diff > std::chrono::milliseconds{500}) {
                active_buffer->released = false;
                surface->attach(active_buffer->id());
                surface->damage();
                surface->commit();
                last_buffer_commit_time = std::chrono::steady_clock::now();

                if (next_buffer_to_use == 0) {
                    next_buffer_to_use = 1;
                } else if (next_buffer_to_use == 1) {
                    next_buffer_to_use = 0;
                }
            }
        }
    }

} catch (std::exception &e) {
    std::println(stderr, "std::exception [{}]", e.what());
    return EXIT_FAILURE;
}
