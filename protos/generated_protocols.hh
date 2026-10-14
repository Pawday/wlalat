#pragma once

#include <tuple>

template<typename PMeta>
struct protocols
{
struct wayland
{
struct wl_display
{
struct message_sync
{
    PMeta::new_id_t callback;
    struct Meta
    {
        using Type = message_sync;
        using InterfaceTag = wl_display;
        static constexpr char name[] = "sync";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::callback,"callback")
        );
    };
};
struct message_get_registry
{
    PMeta::new_id_t registry;
    struct Meta
    {
        using Type = message_get_registry;
        using InterfaceTag = wl_display;
        static constexpr char name[] = "get_registry";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::registry,"registry")
        );
    };
};
struct message_error
{
    PMeta::object_t object_id;
    PMeta::uint_t code;
    PMeta::string_t message;
    struct Meta
    {
        using Type = message_error;
        using InterfaceTag = wl_display;
        static constexpr char name[] = "error";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::object_id,"object_id"),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::code     ,"code"     ),
            std::make_tuple(typename PMeta::string_tag{},&Type::message  ,"message"  )
        );
    };
};
struct message_delete_id
{
    PMeta::uint_t id;
    struct Meta
    {
        using Type = message_delete_id;
        using InterfaceTag = wl_display;
        static constexpr char name[] = "delete_id";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::id,"id")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_display";
    using Events = std::tuple
    <
        message_error,
        message_delete_id
    >;
    using Requests = std::tuple
    <
        message_sync,
        message_get_registry
    >;
};
}; // struct wl_display

struct wl_registry
{
struct message_bind
{
    PMeta::uint_t name;
    PMeta::string_t id_interface;
    PMeta::uint_t id_version;
    PMeta::new_id_t id;
    struct Meta
    {
        using Type = message_bind;
        using InterfaceTag = wl_registry;
        static constexpr char name[] = "bind";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::name        ,"name"        ),
            std::make_tuple(typename PMeta::string_tag{},&Type::id_interface,"id_interface"),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::id_version  ,"id_version"  ),
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id          ,"id"          )
        );
    };
};
struct message_global
{
    PMeta::uint_t name;
    PMeta::string_t interface;
    PMeta::uint_t version;
    struct Meta
    {
        using Type = message_global;
        using InterfaceTag = wl_registry;
        static constexpr char name[] = "global";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::name     ,"name"     ),
            std::make_tuple(typename PMeta::string_tag{},&Type::interface,"interface"),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::version  ,"version"  )
        );
    };
};
struct message_global_remove
{
    PMeta::uint_t name;
    struct Meta
    {
        using Type = message_global_remove;
        using InterfaceTag = wl_registry;
        static constexpr char name[] = "global_remove";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::name,"name")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_registry";
    using Events = std::tuple
    <
        message_global,
        message_global_remove
    >;
    using Requests = std::tuple
    <
        message_bind
    >;
};
}; // struct wl_registry

struct wl_callback
{
struct message_done
{
    PMeta::uint_t callback_data;
    struct Meta
    {
        using Type = message_done;
        using InterfaceTag = wl_callback;
        static constexpr char name[] = "done";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::callback_data,"callback_data")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_callback";
    using Events = std::tuple
    <
        message_done
    >;
    using Requests = std::tuple
    <
    >;
};
}; // struct wl_callback

struct wl_compositor
{
struct message_create_surface
{
    PMeta::new_id_t id;
    struct Meta
    {
        using Type = message_create_surface;
        using InterfaceTag = wl_compositor;
        static constexpr char name[] = "create_surface";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
};
struct message_create_region
{
    PMeta::new_id_t id;
    struct Meta
    {
        using Type = message_create_region;
        using InterfaceTag = wl_compositor;
        static constexpr char name[] = "create_region";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
};
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_compositor;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_compositor";
    using Events = std::tuple
    <
    >;
    using Requests = std::tuple
    <
        message_create_surface,
        message_create_region,
        message_release
    >;
};
}; // struct wl_compositor

struct wl_shm_pool
{
struct message_create_buffer
{
    PMeta::new_id_t id;
    PMeta::int_t offset;
    PMeta::int_t width;
    PMeta::int_t height;
    PMeta::int_t stride;
    PMeta::uint_t format;
    struct Meta
    {
        using Type = message_create_buffer;
        using InterfaceTag = wl_shm_pool;
        static constexpr char name[] = "create_buffer";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id    ,"id"    ),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::offset,"offset"),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::width ,"width" ),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::height,"height"),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::stride,"stride"),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::format,"format")
        );
    };
};
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = wl_shm_pool;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_resize
{
    PMeta::int_t size;
    struct Meta
    {
        using Type = message_resize;
        using InterfaceTag = wl_shm_pool;
        static constexpr char name[] = "resize";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::size,"size")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_shm_pool";
    using Events = std::tuple
    <
    >;
    using Requests = std::tuple
    <
        message_create_buffer,
        message_destroy,
        message_resize
    >;
};
}; // struct wl_shm_pool

struct wl_shm
{
struct message_create_pool
{
    PMeta::new_id_t id;
    PMeta::fd_t fd;
    PMeta::int_t size;
    struct Meta
    {
        using Type = message_create_pool;
        using InterfaceTag = wl_shm;
        static constexpr char name[] = "create_pool";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id  ,"id"  ),
            std::make_tuple(typename PMeta::fd_tag{}    ,&Type::fd  ,"fd"  ),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::size,"size")
        );
    };
};
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_shm;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_format
{
    PMeta::uint_t format;
    struct Meta
    {
        using Type = message_format;
        using InterfaceTag = wl_shm;
        static constexpr char name[] = "format";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::format,"format")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_shm";
    using Events = std::tuple
    <
        message_format
    >;
    using Requests = std::tuple
    <
        message_create_pool,
        message_release
    >;
};
}; // struct wl_shm

struct wl_buffer
{
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = wl_buffer;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_buffer;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_buffer";
    using Events = std::tuple
    <
        message_release
    >;
    using Requests = std::tuple
    <
        message_destroy
    >;
};
}; // struct wl_buffer

struct wl_data_offer
{
struct message_accept
{
    PMeta::uint_t serial;
    PMeta::string_t mime_type;
    struct Meta
    {
        using Type = message_accept;
        using InterfaceTag = wl_data_offer;
        static constexpr char name[] = "accept";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial   ,"serial"   ),
            std::make_tuple(typename PMeta::string_tag{},&Type::mime_type,"mime_type")
        );
    };
};
struct message_receive
{
    PMeta::string_t mime_type;
    PMeta::fd_t fd;
    struct Meta
    {
        using Type = message_receive;
        using InterfaceTag = wl_data_offer;
        static constexpr char name[] = "receive";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::string_tag{},&Type::mime_type,"mime_type"),
            std::make_tuple(typename PMeta::fd_tag{}    ,&Type::fd       ,"fd"       )
        );
    };
};
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = wl_data_offer;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_finish
{
    struct Meta
    {
        using Type = message_finish;
        using InterfaceTag = wl_data_offer;
        static constexpr char name[] = "finish";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_actions
{
    PMeta::uint_t dnd_actions;
    PMeta::uint_t preferred_action;
    struct Meta
    {
        using Type = message_set_actions;
        using InterfaceTag = wl_data_offer;
        static constexpr char name[] = "set_actions";
        static constexpr const unsigned short opcode = 4;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::dnd_actions     ,"dnd_actions"     ),
            std::make_tuple(typename PMeta::uint_tag{},&Type::preferred_action,"preferred_action")
        );
    };
};
struct message_offer
{
    PMeta::string_t mime_type;
    struct Meta
    {
        using Type = message_offer;
        using InterfaceTag = wl_data_offer;
        static constexpr char name[] = "offer";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::string_tag{},&Type::mime_type,"mime_type")
        );
    };
};
struct message_source_actions
{
    PMeta::uint_t source_actions;
    struct Meta
    {
        using Type = message_source_actions;
        using InterfaceTag = wl_data_offer;
        static constexpr char name[] = "source_actions";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::source_actions,"source_actions")
        );
    };
};
struct message_action
{
    PMeta::uint_t dnd_action;
    struct Meta
    {
        using Type = message_action;
        using InterfaceTag = wl_data_offer;
        static constexpr char name[] = "action";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::dnd_action,"dnd_action")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_data_offer";
    using Events = std::tuple
    <
        message_offer,
        message_source_actions,
        message_action
    >;
    using Requests = std::tuple
    <
        message_accept,
        message_receive,
        message_destroy,
        message_finish,
        message_set_actions
    >;
};
}; // struct wl_data_offer

struct wl_data_source
{
struct message_offer
{
    PMeta::string_t mime_type;
    struct Meta
    {
        using Type = message_offer;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "offer";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::string_tag{},&Type::mime_type,"mime_type")
        );
    };
};
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_actions
{
    PMeta::uint_t dnd_actions;
    struct Meta
    {
        using Type = message_set_actions;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "set_actions";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::dnd_actions,"dnd_actions")
        );
    };
};
struct message_target
{
    PMeta::string_t mime_type;
    struct Meta
    {
        using Type = message_target;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "target";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::string_tag{},&Type::mime_type,"mime_type")
        );
    };
};
struct message_send
{
    PMeta::string_t mime_type;
    PMeta::fd_t fd;
    struct Meta
    {
        using Type = message_send;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "send";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::string_tag{},&Type::mime_type,"mime_type"),
            std::make_tuple(typename PMeta::fd_tag{}    ,&Type::fd       ,"fd"       )
        );
    };
};
struct message_cancelled
{
    struct Meta
    {
        using Type = message_cancelled;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "cancelled";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_dnd_drop_performed
{
    struct Meta
    {
        using Type = message_dnd_drop_performed;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "dnd_drop_performed";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_dnd_finished
{
    struct Meta
    {
        using Type = message_dnd_finished;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "dnd_finished";
        static constexpr const unsigned short opcode = 4;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_action
{
    PMeta::uint_t dnd_action;
    struct Meta
    {
        using Type = message_action;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "action";
        static constexpr const unsigned short opcode = 5;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::dnd_action,"dnd_action")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_data_source";
    using Events = std::tuple
    <
        message_target,
        message_send,
        message_cancelled,
        message_dnd_drop_performed,
        message_dnd_finished,
        message_action
    >;
    using Requests = std::tuple
    <
        message_offer,
        message_destroy,
        message_set_actions
    >;
};
}; // struct wl_data_source

struct wl_data_device
{
struct message_start_drag
{
    PMeta::object_t source;
    PMeta::object_t origin;
    PMeta::object_t icon;
    PMeta::uint_t serial;
    struct Meta
    {
        using Type = message_start_drag;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "start_drag";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::source,"source"),
            std::make_tuple(typename PMeta::object_tag{},&Type::origin,"origin"),
            std::make_tuple(typename PMeta::object_tag{},&Type::icon  ,"icon"  ),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial,"serial")
        );
    };
};
struct message_set_selection
{
    PMeta::object_t source;
    PMeta::uint_t serial;
    struct Meta
    {
        using Type = message_set_selection;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "set_selection";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::source,"source"),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial,"serial")
        );
    };
};
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_data_offer
{
    PMeta::new_id_t id;
    struct Meta
    {
        using Type = message_data_offer;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "data_offer";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
};
struct message_enter
{
    PMeta::uint_t serial;
    PMeta::object_t surface;
    PMeta::fixed_t x;
    PMeta::fixed_t y;
    PMeta::object_t id;
    struct Meta
    {
        using Type = message_enter;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "enter";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial ,"serial" ),
            std::make_tuple(typename PMeta::object_tag{},&Type::surface,"surface"),
            std::make_tuple(typename PMeta::fixed_tag{} ,&Type::x      ,"x"      ),
            std::make_tuple(typename PMeta::fixed_tag{} ,&Type::y      ,"y"      ),
            std::make_tuple(typename PMeta::object_tag{},&Type::id     ,"id"     )
        );
    };
};
struct message_leave
{
    struct Meta
    {
        using Type = message_leave;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "leave";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_motion
{
    PMeta::uint_t time;
    PMeta::fixed_t x;
    PMeta::fixed_t y;
    struct Meta
    {
        using Type = message_motion;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "motion";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{} ,&Type::time,"time"),
            std::make_tuple(typename PMeta::fixed_tag{},&Type::x   ,"x"   ),
            std::make_tuple(typename PMeta::fixed_tag{},&Type::y   ,"y"   )
        );
    };
};
struct message_drop
{
    struct Meta
    {
        using Type = message_drop;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "drop";
        static constexpr const unsigned short opcode = 4;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_selection
{
    PMeta::object_t id;
    struct Meta
    {
        using Type = message_selection;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "selection";
        static constexpr const unsigned short opcode = 5;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::id,"id")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_data_device";
    using Events = std::tuple
    <
        message_data_offer,
        message_enter,
        message_leave,
        message_motion,
        message_drop,
        message_selection
    >;
    using Requests = std::tuple
    <
        message_start_drag,
        message_set_selection,
        message_release
    >;
};
}; // struct wl_data_device

struct wl_data_device_manager
{
struct message_create_data_source
{
    PMeta::new_id_t id;
    struct Meta
    {
        using Type = message_create_data_source;
        using InterfaceTag = wl_data_device_manager;
        static constexpr char name[] = "create_data_source";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
};
struct message_get_data_device
{
    PMeta::new_id_t id;
    PMeta::object_t seat;
    struct Meta
    {
        using Type = message_get_data_device;
        using InterfaceTag = wl_data_device_manager;
        static constexpr char name[] = "get_data_device";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id  ,"id"  ),
            std::make_tuple(typename PMeta::object_tag{},&Type::seat,"seat")
        );
    };
};
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_data_device_manager;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_data_device_manager";
    using Events = std::tuple
    <
    >;
    using Requests = std::tuple
    <
        message_create_data_source,
        message_get_data_device,
        message_release
    >;
};
}; // struct wl_data_device_manager

struct wl_shell
{
struct message_get_shell_surface
{
    PMeta::new_id_t id;
    PMeta::object_t surface;
    struct Meta
    {
        using Type = message_get_shell_surface;
        using InterfaceTag = wl_shell;
        static constexpr char name[] = "get_shell_surface";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id     ,"id"     ),
            std::make_tuple(typename PMeta::object_tag{},&Type::surface,"surface")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_shell";
    using Events = std::tuple
    <
    >;
    using Requests = std::tuple
    <
        message_get_shell_surface
    >;
};
}; // struct wl_shell

struct wl_shell_surface
{
struct message_pong
{
    PMeta::uint_t serial;
    struct Meta
    {
        using Type = message_pong;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "pong";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::serial,"serial")
        );
    };
};
struct message_move
{
    PMeta::object_t seat;
    PMeta::uint_t serial;
    struct Meta
    {
        using Type = message_move;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "move";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::seat  ,"seat"  ),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial,"serial")
        );
    };
};
struct message_resize
{
    PMeta::object_t seat;
    PMeta::uint_t serial;
    PMeta::uint_t edges;
    struct Meta
    {
        using Type = message_resize;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "resize";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::seat  ,"seat"  ),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial,"serial"),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::edges ,"edges" )
        );
    };
};
struct message_set_toplevel
{
    struct Meta
    {
        using Type = message_set_toplevel;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "set_toplevel";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_transient
{
    PMeta::object_t parent;
    PMeta::int_t x;
    PMeta::int_t y;
    PMeta::uint_t flags;
    struct Meta
    {
        using Type = message_set_transient;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "set_transient";
        static constexpr const unsigned short opcode = 4;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::parent,"parent"),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::x     ,"x"     ),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::y     ,"y"     ),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::flags ,"flags" )
        );
    };
};
struct message_set_fullscreen
{
    PMeta::uint_t method;
    PMeta::uint_t framerate;
    PMeta::object_t output;
    struct Meta
    {
        using Type = message_set_fullscreen;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "set_fullscreen";
        static constexpr const unsigned short opcode = 5;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::method   ,"method"   ),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::framerate,"framerate"),
            std::make_tuple(typename PMeta::object_tag{},&Type::output   ,"output"   )
        );
    };
};
struct message_set_popup
{
    PMeta::object_t seat;
    PMeta::uint_t serial;
    PMeta::object_t parent;
    PMeta::int_t x;
    PMeta::int_t y;
    PMeta::uint_t flags;
    struct Meta
    {
        using Type = message_set_popup;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "set_popup";
        static constexpr const unsigned short opcode = 6;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::seat  ,"seat"  ),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial,"serial"),
            std::make_tuple(typename PMeta::object_tag{},&Type::parent,"parent"),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::x     ,"x"     ),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::y     ,"y"     ),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::flags ,"flags" )
        );
    };
};
struct message_set_maximized
{
    PMeta::object_t output;
    struct Meta
    {
        using Type = message_set_maximized;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "set_maximized";
        static constexpr const unsigned short opcode = 7;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::output,"output")
        );
    };
};
struct message_set_title
{
    PMeta::string_t title;
    struct Meta
    {
        using Type = message_set_title;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "set_title";
        static constexpr const unsigned short opcode = 8;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::string_tag{},&Type::title,"title")
        );
    };
};
struct message_set_class
{
    PMeta::string_t class_;
    struct Meta
    {
        using Type = message_set_class;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "set_class";
        static constexpr const unsigned short opcode = 9;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::string_tag{},&Type::class_,"class_")
        );
    };
};
struct message_ping
{
    PMeta::uint_t serial;
    struct Meta
    {
        using Type = message_ping;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "ping";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::serial,"serial")
        );
    };
};
struct message_configure
{
    PMeta::uint_t edges;
    PMeta::int_t width;
    PMeta::int_t height;
    struct Meta
    {
        using Type = message_configure;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "configure";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::edges ,"edges" ),
            std::make_tuple(typename PMeta::int_tag{} ,&Type::width ,"width" ),
            std::make_tuple(typename PMeta::int_tag{} ,&Type::height,"height")
        );
    };
};
struct message_popup_done
{
    struct Meta
    {
        using Type = message_popup_done;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "popup_done";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_shell_surface";
    using Events = std::tuple
    <
        message_ping,
        message_configure,
        message_popup_done
    >;
    using Requests = std::tuple
    <
        message_pong,
        message_move,
        message_resize,
        message_set_toplevel,
        message_set_transient,
        message_set_fullscreen,
        message_set_popup,
        message_set_maximized,
        message_set_title,
        message_set_class
    >;
};
}; // struct wl_shell_surface

struct wl_surface
{
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_attach
{
    PMeta::object_t buffer;
    PMeta::int_t x;
    PMeta::int_t y;
    struct Meta
    {
        using Type = message_attach;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "attach";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::buffer,"buffer"),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::x     ,"x"     ),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::y     ,"y"     )
        );
    };
};
struct message_damage
{
    PMeta::int_t x;
    PMeta::int_t y;
    PMeta::int_t width;
    PMeta::int_t height;
    struct Meta
    {
        using Type = message_damage;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "damage";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::x     ,"x"     ),
            std::make_tuple(typename PMeta::int_tag{},&Type::y     ,"y"     ),
            std::make_tuple(typename PMeta::int_tag{},&Type::width ,"width" ),
            std::make_tuple(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
};
struct message_frame
{
    PMeta::new_id_t callback;
    struct Meta
    {
        using Type = message_frame;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "frame";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::callback,"callback")
        );
    };
};
struct message_set_opaque_region
{
    PMeta::object_t region;
    struct Meta
    {
        using Type = message_set_opaque_region;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "set_opaque_region";
        static constexpr const unsigned short opcode = 4;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::region,"region")
        );
    };
};
struct message_set_input_region
{
    PMeta::object_t region;
    struct Meta
    {
        using Type = message_set_input_region;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "set_input_region";
        static constexpr const unsigned short opcode = 5;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::region,"region")
        );
    };
};
struct message_commit
{
    struct Meta
    {
        using Type = message_commit;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "commit";
        static constexpr const unsigned short opcode = 6;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_buffer_transform
{
    PMeta::int_t transform;
    struct Meta
    {
        using Type = message_set_buffer_transform;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "set_buffer_transform";
        static constexpr const unsigned short opcode = 7;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::transform,"transform")
        );
    };
};
struct message_set_buffer_scale
{
    PMeta::int_t scale;
    struct Meta
    {
        using Type = message_set_buffer_scale;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "set_buffer_scale";
        static constexpr const unsigned short opcode = 8;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::scale,"scale")
        );
    };
};
struct message_damage_buffer
{
    PMeta::int_t x;
    PMeta::int_t y;
    PMeta::int_t width;
    PMeta::int_t height;
    struct Meta
    {
        using Type = message_damage_buffer;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "damage_buffer";
        static constexpr const unsigned short opcode = 9;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::x     ,"x"     ),
            std::make_tuple(typename PMeta::int_tag{},&Type::y     ,"y"     ),
            std::make_tuple(typename PMeta::int_tag{},&Type::width ,"width" ),
            std::make_tuple(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
};
struct message_offset
{
    PMeta::int_t x;
    PMeta::int_t y;
    struct Meta
    {
        using Type = message_offset;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "offset";
        static constexpr const unsigned short opcode = 10;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::x,"x"),
            std::make_tuple(typename PMeta::int_tag{},&Type::y,"y")
        );
    };
};
struct message_get_release
{
    PMeta::new_id_t callback;
    struct Meta
    {
        using Type = message_get_release;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "get_release";
        static constexpr const unsigned short opcode = 11;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::callback,"callback")
        );
    };
};
struct message_enter
{
    PMeta::object_t output;
    struct Meta
    {
        using Type = message_enter;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "enter";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::output,"output")
        );
    };
};
struct message_leave
{
    PMeta::object_t output;
    struct Meta
    {
        using Type = message_leave;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "leave";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::output,"output")
        );
    };
};
struct message_preferred_buffer_scale
{
    PMeta::int_t factor;
    struct Meta
    {
        using Type = message_preferred_buffer_scale;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "preferred_buffer_scale";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::factor,"factor")
        );
    };
};
struct message_preferred_buffer_transform
{
    PMeta::uint_t transform;
    struct Meta
    {
        using Type = message_preferred_buffer_transform;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "preferred_buffer_transform";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::transform,"transform")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_surface";
    using Events = std::tuple
    <
        message_enter,
        message_leave,
        message_preferred_buffer_scale,
        message_preferred_buffer_transform
    >;
    using Requests = std::tuple
    <
        message_destroy,
        message_attach,
        message_damage,
        message_frame,
        message_set_opaque_region,
        message_set_input_region,
        message_commit,
        message_set_buffer_transform,
        message_set_buffer_scale,
        message_damage_buffer,
        message_offset,
        message_get_release
    >;
};
}; // struct wl_surface

struct wl_seat
{
struct message_get_pointer
{
    PMeta::new_id_t id;
    struct Meta
    {
        using Type = message_get_pointer;
        using InterfaceTag = wl_seat;
        static constexpr char name[] = "get_pointer";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
};
struct message_get_keyboard
{
    PMeta::new_id_t id;
    struct Meta
    {
        using Type = message_get_keyboard;
        using InterfaceTag = wl_seat;
        static constexpr char name[] = "get_keyboard";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
};
struct message_get_touch
{
    PMeta::new_id_t id;
    struct Meta
    {
        using Type = message_get_touch;
        using InterfaceTag = wl_seat;
        static constexpr char name[] = "get_touch";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
};
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_seat;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_capabilities
{
    PMeta::uint_t capabilities;
    struct Meta
    {
        using Type = message_capabilities;
        using InterfaceTag = wl_seat;
        static constexpr char name[] = "capabilities";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::capabilities,"capabilities")
        );
    };
};
struct message_name
{
    PMeta::string_t name;
    struct Meta
    {
        using Type = message_name;
        using InterfaceTag = wl_seat;
        static constexpr char name[] = "name";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::string_tag{},&Type::name,"name")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_seat";
    using Events = std::tuple
    <
        message_capabilities,
        message_name
    >;
    using Requests = std::tuple
    <
        message_get_pointer,
        message_get_keyboard,
        message_get_touch,
        message_release
    >;
};
}; // struct wl_seat

struct wl_pointer
{
struct message_set_cursor
{
    PMeta::uint_t serial;
    PMeta::object_t surface;
    PMeta::int_t hotspot_x;
    PMeta::int_t hotspot_y;
    struct Meta
    {
        using Type = message_set_cursor;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "set_cursor";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial   ,"serial"   ),
            std::make_tuple(typename PMeta::object_tag{},&Type::surface  ,"surface"  ),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::hotspot_x,"hotspot_x"),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::hotspot_y,"hotspot_y")
        );
    };
};
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_enter
{
    PMeta::uint_t serial;
    PMeta::object_t surface;
    PMeta::fixed_t surface_x;
    PMeta::fixed_t surface_y;
    struct Meta
    {
        using Type = message_enter;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "enter";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial   ,"serial"   ),
            std::make_tuple(typename PMeta::object_tag{},&Type::surface  ,"surface"  ),
            std::make_tuple(typename PMeta::fixed_tag{} ,&Type::surface_x,"surface_x"),
            std::make_tuple(typename PMeta::fixed_tag{} ,&Type::surface_y,"surface_y")
        );
    };
};
struct message_leave
{
    PMeta::uint_t serial;
    PMeta::object_t surface;
    struct Meta
    {
        using Type = message_leave;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "leave";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial ,"serial" ),
            std::make_tuple(typename PMeta::object_tag{},&Type::surface,"surface")
        );
    };
};
struct message_motion
{
    PMeta::uint_t time;
    PMeta::fixed_t surface_x;
    PMeta::fixed_t surface_y;
    struct Meta
    {
        using Type = message_motion;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "motion";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{} ,&Type::time     ,"time"     ),
            std::make_tuple(typename PMeta::fixed_tag{},&Type::surface_x,"surface_x"),
            std::make_tuple(typename PMeta::fixed_tag{},&Type::surface_y,"surface_y")
        );
    };
};
struct message_button
{
    PMeta::uint_t serial;
    PMeta::uint_t time;
    PMeta::uint_t button;
    PMeta::uint_t state;
    struct Meta
    {
        using Type = message_button;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "button";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::serial,"serial"),
            std::make_tuple(typename PMeta::uint_tag{},&Type::time  ,"time"  ),
            std::make_tuple(typename PMeta::uint_tag{},&Type::button,"button"),
            std::make_tuple(typename PMeta::uint_tag{},&Type::state ,"state" )
        );
    };
};
struct message_axis
{
    PMeta::uint_t time;
    PMeta::uint_t axis;
    PMeta::fixed_t value;
    struct Meta
    {
        using Type = message_axis;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "axis";
        static constexpr const unsigned short opcode = 4;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{} ,&Type::time ,"time" ),
            std::make_tuple(typename PMeta::uint_tag{} ,&Type::axis ,"axis" ),
            std::make_tuple(typename PMeta::fixed_tag{},&Type::value,"value")
        );
    };
};
struct message_frame
{
    struct Meta
    {
        using Type = message_frame;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "frame";
        static constexpr const unsigned short opcode = 5;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_axis_source
{
    PMeta::uint_t axis_source;
    struct Meta
    {
        using Type = message_axis_source;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "axis_source";
        static constexpr const unsigned short opcode = 6;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::axis_source,"axis_source")
        );
    };
};
struct message_axis_stop
{
    PMeta::uint_t time;
    PMeta::uint_t axis;
    struct Meta
    {
        using Type = message_axis_stop;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "axis_stop";
        static constexpr const unsigned short opcode = 7;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::time,"time"),
            std::make_tuple(typename PMeta::uint_tag{},&Type::axis,"axis")
        );
    };
};
struct message_axis_discrete
{
    PMeta::uint_t axis;
    PMeta::int_t discrete;
    struct Meta
    {
        using Type = message_axis_discrete;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "axis_discrete";
        static constexpr const unsigned short opcode = 8;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::axis    ,"axis"    ),
            std::make_tuple(typename PMeta::int_tag{} ,&Type::discrete,"discrete")
        );
    };
};
struct message_axis_value120
{
    PMeta::uint_t axis;
    PMeta::int_t value120;
    struct Meta
    {
        using Type = message_axis_value120;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "axis_value120";
        static constexpr const unsigned short opcode = 9;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::axis    ,"axis"    ),
            std::make_tuple(typename PMeta::int_tag{} ,&Type::value120,"value120")
        );
    };
};
struct message_axis_relative_direction
{
    PMeta::uint_t axis;
    PMeta::uint_t direction;
    struct Meta
    {
        using Type = message_axis_relative_direction;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "axis_relative_direction";
        static constexpr const unsigned short opcode = 10;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::axis     ,"axis"     ),
            std::make_tuple(typename PMeta::uint_tag{},&Type::direction,"direction")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_pointer";
    using Events = std::tuple
    <
        message_enter,
        message_leave,
        message_motion,
        message_button,
        message_axis,
        message_frame,
        message_axis_source,
        message_axis_stop,
        message_axis_discrete,
        message_axis_value120,
        message_axis_relative_direction
    >;
    using Requests = std::tuple
    <
        message_set_cursor,
        message_release
    >;
};
}; // struct wl_pointer

struct wl_keyboard
{
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_keyboard;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_keymap
{
    PMeta::uint_t format;
    PMeta::fd_t fd;
    PMeta::uint_t size;
    struct Meta
    {
        using Type = message_keymap;
        using InterfaceTag = wl_keyboard;
        static constexpr char name[] = "keymap";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::format,"format"),
            std::make_tuple(typename PMeta::fd_tag{}  ,&Type::fd    ,"fd"    ),
            std::make_tuple(typename PMeta::uint_tag{},&Type::size  ,"size"  )
        );
    };
};
struct message_enter
{
    PMeta::uint_t serial;
    PMeta::object_t surface;
    PMeta::array_t keys;
    struct Meta
    {
        using Type = message_enter;
        using InterfaceTag = wl_keyboard;
        static constexpr char name[] = "enter";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial ,"serial" ),
            std::make_tuple(typename PMeta::object_tag{},&Type::surface,"surface"),
            std::make_tuple(typename PMeta::array_tag{} ,&Type::keys   ,"keys"   )
        );
    };
};
struct message_leave
{
    PMeta::uint_t serial;
    PMeta::object_t surface;
    struct Meta
    {
        using Type = message_leave;
        using InterfaceTag = wl_keyboard;
        static constexpr char name[] = "leave";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial ,"serial" ),
            std::make_tuple(typename PMeta::object_tag{},&Type::surface,"surface")
        );
    };
};
struct message_key
{
    PMeta::uint_t serial;
    PMeta::uint_t time;
    PMeta::uint_t key;
    PMeta::uint_t state;
    struct Meta
    {
        using Type = message_key;
        using InterfaceTag = wl_keyboard;
        static constexpr char name[] = "key";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::serial,"serial"),
            std::make_tuple(typename PMeta::uint_tag{},&Type::time  ,"time"  ),
            std::make_tuple(typename PMeta::uint_tag{},&Type::key   ,"key"   ),
            std::make_tuple(typename PMeta::uint_tag{},&Type::state ,"state" )
        );
    };
};
struct message_modifiers
{
    PMeta::uint_t serial;
    PMeta::uint_t mods_depressed;
    PMeta::uint_t mods_latched;
    PMeta::uint_t mods_locked;
    PMeta::uint_t group;
    struct Meta
    {
        using Type = message_modifiers;
        using InterfaceTag = wl_keyboard;
        static constexpr char name[] = "modifiers";
        static constexpr const unsigned short opcode = 4;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::serial        ,"serial"        ),
            std::make_tuple(typename PMeta::uint_tag{},&Type::mods_depressed,"mods_depressed"),
            std::make_tuple(typename PMeta::uint_tag{},&Type::mods_latched  ,"mods_latched"  ),
            std::make_tuple(typename PMeta::uint_tag{},&Type::mods_locked   ,"mods_locked"   ),
            std::make_tuple(typename PMeta::uint_tag{},&Type::group         ,"group"         )
        );
    };
};
struct message_repeat_info
{
    PMeta::int_t rate;
    PMeta::int_t delay;
    struct Meta
    {
        using Type = message_repeat_info;
        using InterfaceTag = wl_keyboard;
        static constexpr char name[] = "repeat_info";
        static constexpr const unsigned short opcode = 5;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::rate ,"rate" ),
            std::make_tuple(typename PMeta::int_tag{},&Type::delay,"delay")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_keyboard";
    using Events = std::tuple
    <
        message_keymap,
        message_enter,
        message_leave,
        message_key,
        message_modifiers,
        message_repeat_info
    >;
    using Requests = std::tuple
    <
        message_release
    >;
};
}; // struct wl_keyboard

struct wl_touch
{
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_touch;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_down
{
    PMeta::uint_t serial;
    PMeta::uint_t time;
    PMeta::object_t surface;
    PMeta::int_t id;
    PMeta::fixed_t x;
    PMeta::fixed_t y;
    struct Meta
    {
        using Type = message_down;
        using InterfaceTag = wl_touch;
        static constexpr char name[] = "down";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial ,"serial" ),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::time   ,"time"   ),
            std::make_tuple(typename PMeta::object_tag{},&Type::surface,"surface"),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::id     ,"id"     ),
            std::make_tuple(typename PMeta::fixed_tag{} ,&Type::x      ,"x"      ),
            std::make_tuple(typename PMeta::fixed_tag{} ,&Type::y      ,"y"      )
        );
    };
};
struct message_up
{
    PMeta::uint_t serial;
    PMeta::uint_t time;
    PMeta::int_t id;
    struct Meta
    {
        using Type = message_up;
        using InterfaceTag = wl_touch;
        static constexpr char name[] = "up";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::serial,"serial"),
            std::make_tuple(typename PMeta::uint_tag{},&Type::time  ,"time"  ),
            std::make_tuple(typename PMeta::int_tag{} ,&Type::id    ,"id"    )
        );
    };
};
struct message_motion
{
    PMeta::uint_t time;
    PMeta::int_t id;
    PMeta::fixed_t x;
    PMeta::fixed_t y;
    struct Meta
    {
        using Type = message_motion;
        using InterfaceTag = wl_touch;
        static constexpr char name[] = "motion";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{} ,&Type::time,"time"),
            std::make_tuple(typename PMeta::int_tag{}  ,&Type::id  ,"id"  ),
            std::make_tuple(typename PMeta::fixed_tag{},&Type::x   ,"x"   ),
            std::make_tuple(typename PMeta::fixed_tag{},&Type::y   ,"y"   )
        );
    };
};
struct message_frame
{
    struct Meta
    {
        using Type = message_frame;
        using InterfaceTag = wl_touch;
        static constexpr char name[] = "frame";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_cancel
{
    struct Meta
    {
        using Type = message_cancel;
        using InterfaceTag = wl_touch;
        static constexpr char name[] = "cancel";
        static constexpr const unsigned short opcode = 4;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_shape
{
    PMeta::int_t id;
    PMeta::fixed_t major;
    PMeta::fixed_t minor;
    struct Meta
    {
        using Type = message_shape;
        using InterfaceTag = wl_touch;
        static constexpr char name[] = "shape";
        static constexpr const unsigned short opcode = 5;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{}  ,&Type::id   ,"id"   ),
            std::make_tuple(typename PMeta::fixed_tag{},&Type::major,"major"),
            std::make_tuple(typename PMeta::fixed_tag{},&Type::minor,"minor")
        );
    };
};
struct message_orientation
{
    PMeta::int_t id;
    PMeta::fixed_t orientation;
    struct Meta
    {
        using Type = message_orientation;
        using InterfaceTag = wl_touch;
        static constexpr char name[] = "orientation";
        static constexpr const unsigned short opcode = 6;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{}  ,&Type::id         ,"id"         ),
            std::make_tuple(typename PMeta::fixed_tag{},&Type::orientation,"orientation")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_touch";
    using Events = std::tuple
    <
        message_down,
        message_up,
        message_motion,
        message_frame,
        message_cancel,
        message_shape,
        message_orientation
    >;
    using Requests = std::tuple
    <
        message_release
    >;
};
}; // struct wl_touch

struct wl_output
{
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_output;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_geometry
{
    PMeta::int_t x;
    PMeta::int_t y;
    PMeta::int_t physical_width;
    PMeta::int_t physical_height;
    PMeta::int_t subpixel;
    PMeta::string_t make;
    PMeta::string_t model;
    PMeta::int_t transform;
    struct Meta
    {
        using Type = message_geometry;
        using InterfaceTag = wl_output;
        static constexpr char name[] = "geometry";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::x              ,"x"              ),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::y              ,"y"              ),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::physical_width ,"physical_width" ),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::physical_height,"physical_height"),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::subpixel       ,"subpixel"       ),
            std::make_tuple(typename PMeta::string_tag{},&Type::make           ,"make"           ),
            std::make_tuple(typename PMeta::string_tag{},&Type::model          ,"model"          ),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::transform      ,"transform"      )
        );
    };
};
struct message_mode
{
    PMeta::uint_t flags;
    PMeta::int_t width;
    PMeta::int_t height;
    PMeta::int_t refresh;
    struct Meta
    {
        using Type = message_mode;
        using InterfaceTag = wl_output;
        static constexpr char name[] = "mode";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::flags  ,"flags"  ),
            std::make_tuple(typename PMeta::int_tag{} ,&Type::width  ,"width"  ),
            std::make_tuple(typename PMeta::int_tag{} ,&Type::height ,"height" ),
            std::make_tuple(typename PMeta::int_tag{} ,&Type::refresh,"refresh")
        );
    };
};
struct message_done
{
    struct Meta
    {
        using Type = message_done;
        using InterfaceTag = wl_output;
        static constexpr char name[] = "done";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_scale
{
    PMeta::int_t factor;
    struct Meta
    {
        using Type = message_scale;
        using InterfaceTag = wl_output;
        static constexpr char name[] = "scale";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::factor,"factor")
        );
    };
};
struct message_name
{
    PMeta::string_t name;
    struct Meta
    {
        using Type = message_name;
        using InterfaceTag = wl_output;
        static constexpr char name[] = "name";
        static constexpr const unsigned short opcode = 4;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::string_tag{},&Type::name,"name")
        );
    };
};
struct message_description
{
    PMeta::string_t description;
    struct Meta
    {
        using Type = message_description;
        using InterfaceTag = wl_output;
        static constexpr char name[] = "description";
        static constexpr const unsigned short opcode = 5;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::string_tag{},&Type::description,"description")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_output";
    using Events = std::tuple
    <
        message_geometry,
        message_mode,
        message_done,
        message_scale,
        message_name,
        message_description
    >;
    using Requests = std::tuple
    <
        message_release
    >;
};
}; // struct wl_output

struct wl_region
{
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = wl_region;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_add
{
    PMeta::int_t x;
    PMeta::int_t y;
    PMeta::int_t width;
    PMeta::int_t height;
    struct Meta
    {
        using Type = message_add;
        using InterfaceTag = wl_region;
        static constexpr char name[] = "add";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::x     ,"x"     ),
            std::make_tuple(typename PMeta::int_tag{},&Type::y     ,"y"     ),
            std::make_tuple(typename PMeta::int_tag{},&Type::width ,"width" ),
            std::make_tuple(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
};
struct message_subtract
{
    PMeta::int_t x;
    PMeta::int_t y;
    PMeta::int_t width;
    PMeta::int_t height;
    struct Meta
    {
        using Type = message_subtract;
        using InterfaceTag = wl_region;
        static constexpr char name[] = "subtract";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::x     ,"x"     ),
            std::make_tuple(typename PMeta::int_tag{},&Type::y     ,"y"     ),
            std::make_tuple(typename PMeta::int_tag{},&Type::width ,"width" ),
            std::make_tuple(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_region";
    using Events = std::tuple
    <
    >;
    using Requests = std::tuple
    <
        message_destroy,
        message_add,
        message_subtract
    >;
};
}; // struct wl_region

struct wl_subcompositor
{
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = wl_subcompositor;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_get_subsurface
{
    PMeta::new_id_t id;
    PMeta::object_t surface;
    PMeta::object_t parent;
    struct Meta
    {
        using Type = message_get_subsurface;
        using InterfaceTag = wl_subcompositor;
        static constexpr char name[] = "get_subsurface";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id     ,"id"     ),
            std::make_tuple(typename PMeta::object_tag{},&Type::surface,"surface"),
            std::make_tuple(typename PMeta::object_tag{},&Type::parent ,"parent" )
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_subcompositor";
    using Events = std::tuple
    <
    >;
    using Requests = std::tuple
    <
        message_destroy,
        message_get_subsurface
    >;
};
}; // struct wl_subcompositor

struct wl_subsurface
{
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = wl_subsurface;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_position
{
    PMeta::int_t x;
    PMeta::int_t y;
    struct Meta
    {
        using Type = message_set_position;
        using InterfaceTag = wl_subsurface;
        static constexpr char name[] = "set_position";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::x,"x"),
            std::make_tuple(typename PMeta::int_tag{},&Type::y,"y")
        );
    };
};
struct message_place_above
{
    PMeta::object_t sibling;
    struct Meta
    {
        using Type = message_place_above;
        using InterfaceTag = wl_subsurface;
        static constexpr char name[] = "place_above";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::sibling,"sibling")
        );
    };
};
struct message_place_below
{
    PMeta::object_t sibling;
    struct Meta
    {
        using Type = message_place_below;
        using InterfaceTag = wl_subsurface;
        static constexpr char name[] = "place_below";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::sibling,"sibling")
        );
    };
};
struct message_set_sync
{
    struct Meta
    {
        using Type = message_set_sync;
        using InterfaceTag = wl_subsurface;
        static constexpr char name[] = "set_sync";
        static constexpr const unsigned short opcode = 4;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_desync
{
    struct Meta
    {
        using Type = message_set_desync;
        using InterfaceTag = wl_subsurface;
        static constexpr char name[] = "set_desync";
        static constexpr const unsigned short opcode = 5;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_subsurface";
    using Events = std::tuple
    <
    >;
    using Requests = std::tuple
    <
        message_destroy,
        message_set_position,
        message_place_above,
        message_place_below,
        message_set_sync,
        message_set_desync
    >;
};
}; // struct wl_subsurface

struct wl_fixes
{
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = wl_fixes;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_destroy_registry
{
    PMeta::object_t registry;
    struct Meta
    {
        using Type = message_destroy_registry;
        using InterfaceTag = wl_fixes;
        static constexpr char name[] = "destroy_registry";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::registry,"registry")
        );
    };
};
struct message_ack_global_remove
{
    PMeta::object_t registry;
    PMeta::uint_t name;
    struct Meta
    {
        using Type = message_ack_global_remove;
        using InterfaceTag = wl_fixes;
        static constexpr char name[] = "ack_global_remove";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::registry,"registry"),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::name    ,"name"    )
        );
    };
};
struct Meta
{
    static constexpr char name[] = "wl_fixes";
    using Events = std::tuple
    <
    >;
    using Requests = std::tuple
    <
        message_destroy,
        message_destroy_registry,
        message_ack_global_remove
    >;
};
}; // struct wl_fixes
}; // struct wayland
struct xdg_decoration_unstable_v1
{
struct zxdg_decoration_manager_v1
{
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = zxdg_decoration_manager_v1;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_get_toplevel_decoration
{
    PMeta::new_id_t id;
    PMeta::object_t toplevel;
    struct Meta
    {
        using Type = message_get_toplevel_decoration;
        using InterfaceTag = zxdg_decoration_manager_v1;
        static constexpr char name[] = "get_toplevel_decoration";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id      ,"id"      ),
            std::make_tuple(typename PMeta::object_tag{},&Type::toplevel,"toplevel")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "zxdg_decoration_manager_v1";
    using Events = std::tuple
    <
    >;
    using Requests = std::tuple
    <
        message_destroy,
        message_get_toplevel_decoration
    >;
};
}; // struct zxdg_decoration_manager_v1

struct zxdg_toplevel_decoration_v1
{
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = zxdg_toplevel_decoration_v1;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_mode
{
    PMeta::uint_t mode;
    struct Meta
    {
        using Type = message_set_mode;
        using InterfaceTag = zxdg_toplevel_decoration_v1;
        static constexpr char name[] = "set_mode";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::mode,"mode")
        );
    };
};
struct message_unset_mode
{
    struct Meta
    {
        using Type = message_unset_mode;
        using InterfaceTag = zxdg_toplevel_decoration_v1;
        static constexpr char name[] = "unset_mode";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_configure
{
    PMeta::uint_t mode;
    struct Meta
    {
        using Type = message_configure;
        using InterfaceTag = zxdg_toplevel_decoration_v1;
        static constexpr char name[] = "configure";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::mode,"mode")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "zxdg_toplevel_decoration_v1";
    using Events = std::tuple
    <
        message_configure
    >;
    using Requests = std::tuple
    <
        message_destroy,
        message_set_mode,
        message_unset_mode
    >;
};
}; // struct zxdg_toplevel_decoration_v1
}; // struct xdg_decoration_unstable_v1
struct xdg_shell
{
struct xdg_wm_base
{
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = xdg_wm_base;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_create_positioner
{
    PMeta::new_id_t id;
    struct Meta
    {
        using Type = message_create_positioner;
        using InterfaceTag = xdg_wm_base;
        static constexpr char name[] = "create_positioner";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
};
struct message_get_xdg_surface
{
    PMeta::new_id_t id;
    PMeta::object_t surface;
    struct Meta
    {
        using Type = message_get_xdg_surface;
        using InterfaceTag = xdg_wm_base;
        static constexpr char name[] = "get_xdg_surface";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id     ,"id"     ),
            std::make_tuple(typename PMeta::object_tag{},&Type::surface,"surface")
        );
    };
};
struct message_pong
{
    PMeta::uint_t serial;
    struct Meta
    {
        using Type = message_pong;
        using InterfaceTag = xdg_wm_base;
        static constexpr char name[] = "pong";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::serial,"serial")
        );
    };
};
struct message_ping
{
    PMeta::uint_t serial;
    struct Meta
    {
        using Type = message_ping;
        using InterfaceTag = xdg_wm_base;
        static constexpr char name[] = "ping";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::serial,"serial")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "xdg_wm_base";
    using Events = std::tuple
    <
        message_ping
    >;
    using Requests = std::tuple
    <
        message_destroy,
        message_create_positioner,
        message_get_xdg_surface,
        message_pong
    >;
};
}; // struct xdg_wm_base

struct xdg_positioner
{
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_size
{
    PMeta::int_t width;
    PMeta::int_t height;
    struct Meta
    {
        using Type = message_set_size;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_size";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::width ,"width" ),
            std::make_tuple(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
};
struct message_set_anchor_rect
{
    PMeta::int_t x;
    PMeta::int_t y;
    PMeta::int_t width;
    PMeta::int_t height;
    struct Meta
    {
        using Type = message_set_anchor_rect;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_anchor_rect";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::x     ,"x"     ),
            std::make_tuple(typename PMeta::int_tag{},&Type::y     ,"y"     ),
            std::make_tuple(typename PMeta::int_tag{},&Type::width ,"width" ),
            std::make_tuple(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
};
struct message_set_anchor
{
    PMeta::uint_t anchor;
    struct Meta
    {
        using Type = message_set_anchor;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_anchor";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::anchor,"anchor")
        );
    };
};
struct message_set_gravity
{
    PMeta::uint_t gravity;
    struct Meta
    {
        using Type = message_set_gravity;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_gravity";
        static constexpr const unsigned short opcode = 4;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::gravity,"gravity")
        );
    };
};
struct message_set_constraint_adjustment
{
    PMeta::uint_t constraint_adjustment;
    struct Meta
    {
        using Type = message_set_constraint_adjustment;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_constraint_adjustment";
        static constexpr const unsigned short opcode = 5;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::constraint_adjustment,"constraint_adjustment")
        );
    };
};
struct message_set_offset
{
    PMeta::int_t x;
    PMeta::int_t y;
    struct Meta
    {
        using Type = message_set_offset;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_offset";
        static constexpr const unsigned short opcode = 6;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::x,"x"),
            std::make_tuple(typename PMeta::int_tag{},&Type::y,"y")
        );
    };
};
struct message_set_reactive
{
    struct Meta
    {
        using Type = message_set_reactive;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_reactive";
        static constexpr const unsigned short opcode = 7;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_parent_size
{
    PMeta::int_t parent_width;
    PMeta::int_t parent_height;
    struct Meta
    {
        using Type = message_set_parent_size;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_parent_size";
        static constexpr const unsigned short opcode = 8;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::parent_width ,"parent_width" ),
            std::make_tuple(typename PMeta::int_tag{},&Type::parent_height,"parent_height")
        );
    };
};
struct message_set_parent_configure
{
    PMeta::uint_t serial;
    struct Meta
    {
        using Type = message_set_parent_configure;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_parent_configure";
        static constexpr const unsigned short opcode = 9;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::serial,"serial")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "xdg_positioner";
    using Events = std::tuple
    <
    >;
    using Requests = std::tuple
    <
        message_destroy,
        message_set_size,
        message_set_anchor_rect,
        message_set_anchor,
        message_set_gravity,
        message_set_constraint_adjustment,
        message_set_offset,
        message_set_reactive,
        message_set_parent_size,
        message_set_parent_configure
    >;
};
}; // struct xdg_positioner

struct xdg_surface
{
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = xdg_surface;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_get_toplevel
{
    PMeta::new_id_t id;
    struct Meta
    {
        using Type = message_get_toplevel;
        using InterfaceTag = xdg_surface;
        static constexpr char name[] = "get_toplevel";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
};
struct message_get_popup
{
    PMeta::new_id_t id;
    PMeta::object_t parent;
    PMeta::object_t positioner;
    struct Meta
    {
        using Type = message_get_popup;
        using InterfaceTag = xdg_surface;
        static constexpr char name[] = "get_popup";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::new_id_tag{},&Type::id        ,"id"        ),
            std::make_tuple(typename PMeta::object_tag{},&Type::parent    ,"parent"    ),
            std::make_tuple(typename PMeta::object_tag{},&Type::positioner,"positioner")
        );
    };
};
struct message_set_window_geometry
{
    PMeta::int_t x;
    PMeta::int_t y;
    PMeta::int_t width;
    PMeta::int_t height;
    struct Meta
    {
        using Type = message_set_window_geometry;
        using InterfaceTag = xdg_surface;
        static constexpr char name[] = "set_window_geometry";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::x     ,"x"     ),
            std::make_tuple(typename PMeta::int_tag{},&Type::y     ,"y"     ),
            std::make_tuple(typename PMeta::int_tag{},&Type::width ,"width" ),
            std::make_tuple(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
};
struct message_ack_configure
{
    PMeta::uint_t serial;
    struct Meta
    {
        using Type = message_ack_configure;
        using InterfaceTag = xdg_surface;
        static constexpr char name[] = "ack_configure";
        static constexpr const unsigned short opcode = 4;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::serial,"serial")
        );
    };
};
struct message_configure
{
    PMeta::uint_t serial;
    struct Meta
    {
        using Type = message_configure;
        using InterfaceTag = xdg_surface;
        static constexpr char name[] = "configure";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::serial,"serial")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "xdg_surface";
    using Events = std::tuple
    <
        message_configure
    >;
    using Requests = std::tuple
    <
        message_destroy,
        message_get_toplevel,
        message_get_popup,
        message_set_window_geometry,
        message_ack_configure
    >;
};
}; // struct xdg_surface

struct xdg_toplevel
{
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_parent
{
    PMeta::object_t parent;
    struct Meta
    {
        using Type = message_set_parent;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "set_parent";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::parent,"parent")
        );
    };
};
struct message_set_title
{
    PMeta::string_t title;
    struct Meta
    {
        using Type = message_set_title;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "set_title";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::string_tag{},&Type::title,"title")
        );
    };
};
struct message_set_app_id
{
    PMeta::string_t app_id;
    struct Meta
    {
        using Type = message_set_app_id;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "set_app_id";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::string_tag{},&Type::app_id,"app_id")
        );
    };
};
struct message_show_window_menu
{
    PMeta::object_t seat;
    PMeta::uint_t serial;
    PMeta::int_t x;
    PMeta::int_t y;
    struct Meta
    {
        using Type = message_show_window_menu;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "show_window_menu";
        static constexpr const unsigned short opcode = 4;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::seat  ,"seat"  ),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial,"serial"),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::x     ,"x"     ),
            std::make_tuple(typename PMeta::int_tag{}   ,&Type::y     ,"y"     )
        );
    };
};
struct message_move
{
    PMeta::object_t seat;
    PMeta::uint_t serial;
    struct Meta
    {
        using Type = message_move;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "move";
        static constexpr const unsigned short opcode = 5;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::seat  ,"seat"  ),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial,"serial")
        );
    };
};
struct message_resize
{
    PMeta::object_t seat;
    PMeta::uint_t serial;
    PMeta::uint_t edges;
    struct Meta
    {
        using Type = message_resize;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "resize";
        static constexpr const unsigned short opcode = 6;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::seat  ,"seat"  ),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial,"serial"),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::edges ,"edges" )
        );
    };
};
struct message_set_max_size
{
    PMeta::int_t width;
    PMeta::int_t height;
    struct Meta
    {
        using Type = message_set_max_size;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "set_max_size";
        static constexpr const unsigned short opcode = 7;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::width ,"width" ),
            std::make_tuple(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
};
struct message_set_min_size
{
    PMeta::int_t width;
    PMeta::int_t height;
    struct Meta
    {
        using Type = message_set_min_size;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "set_min_size";
        static constexpr const unsigned short opcode = 8;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::width ,"width" ),
            std::make_tuple(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
};
struct message_set_maximized
{
    struct Meta
    {
        using Type = message_set_maximized;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "set_maximized";
        static constexpr const unsigned short opcode = 9;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_unset_maximized
{
    struct Meta
    {
        using Type = message_unset_maximized;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "unset_maximized";
        static constexpr const unsigned short opcode = 10;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_fullscreen
{
    PMeta::object_t output;
    struct Meta
    {
        using Type = message_set_fullscreen;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "set_fullscreen";
        static constexpr const unsigned short opcode = 11;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::output,"output")
        );
    };
};
struct message_unset_fullscreen
{
    struct Meta
    {
        using Type = message_unset_fullscreen;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "unset_fullscreen";
        static constexpr const unsigned short opcode = 12;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_minimized
{
    struct Meta
    {
        using Type = message_set_minimized;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "set_minimized";
        static constexpr const unsigned short opcode = 13;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_configure
{
    PMeta::int_t width;
    PMeta::int_t height;
    PMeta::array_t states;
    struct Meta
    {
        using Type = message_configure;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "configure";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{}  ,&Type::width ,"width" ),
            std::make_tuple(typename PMeta::int_tag{}  ,&Type::height,"height"),
            std::make_tuple(typename PMeta::array_tag{},&Type::states,"states")
        );
    };
};
struct message_close
{
    struct Meta
    {
        using Type = message_close;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "close";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_configure_bounds
{
    PMeta::int_t width;
    PMeta::int_t height;
    struct Meta
    {
        using Type = message_configure_bounds;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "configure_bounds";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::width ,"width" ),
            std::make_tuple(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
};
struct message_wm_capabilities
{
    PMeta::array_t capabilities;
    struct Meta
    {
        using Type = message_wm_capabilities;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "wm_capabilities";
        static constexpr const unsigned short opcode = 3;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::array_tag{},&Type::capabilities,"capabilities")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "xdg_toplevel";
    using Events = std::tuple
    <
        message_configure,
        message_close,
        message_configure_bounds,
        message_wm_capabilities
    >;
    using Requests = std::tuple
    <
        message_destroy,
        message_set_parent,
        message_set_title,
        message_set_app_id,
        message_show_window_menu,
        message_move,
        message_resize,
        message_set_max_size,
        message_set_min_size,
        message_set_maximized,
        message_unset_maximized,
        message_set_fullscreen,
        message_unset_fullscreen,
        message_set_minimized
    >;
};
}; // struct xdg_toplevel

struct xdg_popup
{
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = xdg_popup;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_grab
{
    PMeta::object_t seat;
    PMeta::uint_t serial;
    struct Meta
    {
        using Type = message_grab;
        using InterfaceTag = xdg_popup;
        static constexpr char name[] = "grab";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::seat  ,"seat"  ),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::serial,"serial")
        );
    };
};
struct message_reposition
{
    PMeta::object_t positioner;
    PMeta::uint_t token;
    struct Meta
    {
        using Type = message_reposition;
        using InterfaceTag = xdg_popup;
        static constexpr char name[] = "reposition";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::object_tag{},&Type::positioner,"positioner"),
            std::make_tuple(typename PMeta::uint_tag{}  ,&Type::token     ,"token"     )
        );
    };
};
struct message_configure
{
    PMeta::int_t x;
    PMeta::int_t y;
    PMeta::int_t width;
    PMeta::int_t height;
    struct Meta
    {
        using Type = message_configure;
        using InterfaceTag = xdg_popup;
        static constexpr char name[] = "configure";
        static constexpr const unsigned short opcode = 0;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::int_tag{},&Type::x     ,"x"     ),
            std::make_tuple(typename PMeta::int_tag{},&Type::y     ,"y"     ),
            std::make_tuple(typename PMeta::int_tag{},&Type::width ,"width" ),
            std::make_tuple(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
};
struct message_popup_done
{
    struct Meta
    {
        using Type = message_popup_done;
        using InterfaceTag = xdg_popup;
        static constexpr char name[] = "popup_done";
        static constexpr const unsigned short opcode = 1;
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_repositioned
{
    PMeta::uint_t token;
    struct Meta
    {
        using Type = message_repositioned;
        using InterfaceTag = xdg_popup;
        static constexpr char name[] = "repositioned";
        static constexpr const unsigned short opcode = 2;
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename PMeta::uint_tag{},&Type::token,"token")
        );
    };
};
struct Meta
{
    static constexpr char name[] = "xdg_popup";
    using Events = std::tuple
    <
        message_configure,
        message_popup_done,
        message_repositioned
    >;
    using Requests = std::tuple
    <
        message_destroy,
        message_grab,
        message_reposition
    >;
};
}; // struct xdg_popup
}; // struct xdg_shell
}; // struct protocols
