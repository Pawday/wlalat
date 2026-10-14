#pragma once

#include <cstddef>
#include <cstdint>

#include <string_view>
#include <tuple>
#include <span>

template<typename PMeta>
struct protocols
{
struct wayland
{
struct wl_display
{
struct message_sync
{
    std::uint_least32_t callback;
    struct Meta
    {
        using Type = message_sync;
        using InterfaceTag = wl_display;
        static constexpr char name[] = "sync";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::callback,"callback")
        );
    };
};
struct message_get_registry
{
    std::uint_least32_t registry;
    struct Meta
    {
        using Type = message_get_registry;
        using InterfaceTag = wl_display;
        static constexpr char name[] = "get_registry";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::registry,"registry")
        );
    };
};
struct message_error
{
    std::uint_least32_t object_id;
    std::uint_least32_t code;
    std::string_view message;
    struct Meta
    {
        using Type = message_error;
        using InterfaceTag = wl_display;
        static constexpr char name[] = "error";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::object_id,"object_id"),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::code     ,"code"     ),
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::message  ,"message"  )
        );
    };
};
struct message_delete_id
{
    std::uint_least32_t id;
    struct Meta
    {
        using Type = message_delete_id;
        using InterfaceTag = wl_display;
        static constexpr char name[] = "delete_id";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::id,"id")
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
    std::uint_least32_t name;
    std::string_view id_interface;
    std::uint_least32_t id_version;
    std::uint_least32_t id;
    struct Meta
    {
        using Type = message_bind;
        using InterfaceTag = wl_registry;
        static constexpr char name[] = "bind";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::name        ,"name"        ),
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::id_interface,"id_interface"),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::id_version  ,"id_version"  ),
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id          ,"id"          )
        );
    };
};
struct message_global
{
    std::uint_least32_t name;
    std::string_view interface;
    std::uint_least32_t version;
    struct Meta
    {
        using Type = message_global;
        using InterfaceTag = wl_registry;
        static constexpr char name[] = "global";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::name     ,"name"     ),
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::interface,"interface"),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::version  ,"version"  )
        );
    };
};
struct message_global_remove
{
    std::uint_least32_t name;
    struct Meta
    {
        using Type = message_global_remove;
        using InterfaceTag = wl_registry;
        static constexpr char name[] = "global_remove";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::name,"name")
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
    std::uint_least32_t callback_data;
    struct Meta
    {
        using Type = message_done;
        using InterfaceTag = wl_callback;
        static constexpr char name[] = "done";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::callback_data,"callback_data")
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
    std::uint_least32_t id;
    struct Meta
    {
        using Type = message_create_surface;
        using InterfaceTag = wl_compositor;
        static constexpr char name[] = "create_surface";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id,"id")
        );
    };
};
struct message_create_region
{
    std::uint_least32_t id;
    struct Meta
    {
        using Type = message_create_region;
        using InterfaceTag = wl_compositor;
        static constexpr char name[] = "create_region";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id,"id")
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
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
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
    std::uint_least32_t id;
    std::int_least32_t offset;
    std::int_least32_t width;
    std::int_least32_t height;
    std::int_least32_t stride;
    std::uint_least32_t format;
    struct Meta
    {
        using Type = message_create_buffer;
        using InterfaceTag = wl_shm_pool;
        static constexpr char name[] = "create_buffer";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id    ,"id"    ),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::offset,"offset"),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::width ,"width" ),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::height,"height"),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::stride,"stride"),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::format,"format")
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
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_resize
{
    std::int_least32_t size;
    struct Meta
    {
        using Type = message_resize;
        using InterfaceTag = wl_shm_pool;
        static constexpr char name[] = "resize";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::size,"size")
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
    std::uint_least32_t id;
    void * fd;
    std::int_least32_t size;
    struct Meta
    {
        using Type = message_create_pool;
        using InterfaceTag = wl_shm;
        static constexpr char name[] = "create_pool";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id  ,"id"  ),
            std::make_tuple(typename TypeTagsT::wl_fd{}    ,&Type::fd  ,"fd"  ),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::size,"size")
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
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_format
{
    std::uint_least32_t format;
    struct Meta
    {
        using Type = message_format;
        using InterfaceTag = wl_shm;
        static constexpr char name[] = "format";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::format,"format")
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
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
    std::uint_least32_t serial;
    std::string_view mime_type;
    struct Meta
    {
        using Type = message_accept;
        using InterfaceTag = wl_data_offer;
        static constexpr char name[] = "accept";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial   ,"serial"   ),
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::mime_type,"mime_type")
        );
    };
};
struct message_receive
{
    std::string_view mime_type;
    void * fd;
    struct Meta
    {
        using Type = message_receive;
        using InterfaceTag = wl_data_offer;
        static constexpr char name[] = "receive";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::mime_type,"mime_type"),
            std::make_tuple(typename TypeTagsT::wl_fd{}    ,&Type::fd       ,"fd"       )
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
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
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
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_actions
{
    std::uint_least32_t dnd_actions;
    std::uint_least32_t preferred_action;
    struct Meta
    {
        using Type = message_set_actions;
        using InterfaceTag = wl_data_offer;
        static constexpr char name[] = "set_actions";
        static constexpr const size_t opcode = 4;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::dnd_actions     ,"dnd_actions"     ),
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::preferred_action,"preferred_action")
        );
    };
};
struct message_offer
{
    std::string_view mime_type;
    struct Meta
    {
        using Type = message_offer;
        using InterfaceTag = wl_data_offer;
        static constexpr char name[] = "offer";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::mime_type,"mime_type")
        );
    };
};
struct message_source_actions
{
    std::uint_least32_t source_actions;
    struct Meta
    {
        using Type = message_source_actions;
        using InterfaceTag = wl_data_offer;
        static constexpr char name[] = "source_actions";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::source_actions,"source_actions")
        );
    };
};
struct message_action
{
    std::uint_least32_t dnd_action;
    struct Meta
    {
        using Type = message_action;
        using InterfaceTag = wl_data_offer;
        static constexpr char name[] = "action";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::dnd_action,"dnd_action")
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
    std::string_view mime_type;
    struct Meta
    {
        using Type = message_offer;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "offer";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::mime_type,"mime_type")
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
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_actions
{
    std::uint_least32_t dnd_actions;
    struct Meta
    {
        using Type = message_set_actions;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "set_actions";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::dnd_actions,"dnd_actions")
        );
    };
};
struct message_target
{
    std::string_view mime_type;
    struct Meta
    {
        using Type = message_target;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "target";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::mime_type,"mime_type")
        );
    };
};
struct message_send
{
    std::string_view mime_type;
    void * fd;
    struct Meta
    {
        using Type = message_send;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "send";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::mime_type,"mime_type"),
            std::make_tuple(typename TypeTagsT::wl_fd{}    ,&Type::fd       ,"fd"       )
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
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
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
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
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
        static constexpr const size_t opcode = 4;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_action
{
    std::uint_least32_t dnd_action;
    struct Meta
    {
        using Type = message_action;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "action";
        static constexpr const size_t opcode = 5;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::dnd_action,"dnd_action")
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
    std::uint_least32_t source;
    std::uint_least32_t origin;
    std::uint_least32_t icon;
    std::uint_least32_t serial;
    struct Meta
    {
        using Type = message_start_drag;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "start_drag";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::source,"source"),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::origin,"origin"),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::icon  ,"icon"  ),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial,"serial")
        );
    };
};
struct message_set_selection
{
    std::uint_least32_t source;
    std::uint_least32_t serial;
    struct Meta
    {
        using Type = message_set_selection;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "set_selection";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::source,"source"),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial,"serial")
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
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_data_offer
{
    std::uint_least32_t id;
    struct Meta
    {
        using Type = message_data_offer;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "data_offer";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id,"id")
        );
    };
};
struct message_enter
{
    std::uint_least32_t serial;
    std::uint_least32_t surface;
    std::uint_least32_t x;
    std::uint_least32_t y;
    std::uint_least32_t id;
    struct Meta
    {
        using Type = message_enter;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "enter";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial ,"serial" ),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::surface,"surface"),
            std::make_tuple(typename TypeTagsT::wl_fixed{} ,&Type::x      ,"x"      ),
            std::make_tuple(typename TypeTagsT::wl_fixed{} ,&Type::y      ,"y"      ),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::id     ,"id"     )
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
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_motion
{
    std::uint_least32_t time;
    std::uint_least32_t x;
    std::uint_least32_t y;
    struct Meta
    {
        using Type = message_motion;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "motion";
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{} ,&Type::time,"time"),
            std::make_tuple(typename TypeTagsT::wl_fixed{},&Type::x   ,"x"   ),
            std::make_tuple(typename TypeTagsT::wl_fixed{},&Type::y   ,"y"   )
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
        static constexpr const size_t opcode = 4;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_selection
{
    std::uint_least32_t id;
    struct Meta
    {
        using Type = message_selection;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "selection";
        static constexpr const size_t opcode = 5;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::id,"id")
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
    std::uint_least32_t id;
    struct Meta
    {
        using Type = message_create_data_source;
        using InterfaceTag = wl_data_device_manager;
        static constexpr char name[] = "create_data_source";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id,"id")
        );
    };
};
struct message_get_data_device
{
    std::uint_least32_t id;
    std::uint_least32_t seat;
    struct Meta
    {
        using Type = message_get_data_device;
        using InterfaceTag = wl_data_device_manager;
        static constexpr char name[] = "get_data_device";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id  ,"id"  ),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::seat,"seat")
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
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
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
    std::uint_least32_t id;
    std::uint_least32_t surface;
    struct Meta
    {
        using Type = message_get_shell_surface;
        using InterfaceTag = wl_shell;
        static constexpr char name[] = "get_shell_surface";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id     ,"id"     ),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::surface,"surface")
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
    std::uint_least32_t serial;
    struct Meta
    {
        using Type = message_pong;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "pong";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::serial,"serial")
        );
    };
};
struct message_move
{
    std::uint_least32_t seat;
    std::uint_least32_t serial;
    struct Meta
    {
        using Type = message_move;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "move";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::seat  ,"seat"  ),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial,"serial")
        );
    };
};
struct message_resize
{
    std::uint_least32_t seat;
    std::uint_least32_t serial;
    std::uint_least32_t edges;
    struct Meta
    {
        using Type = message_resize;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "resize";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::seat  ,"seat"  ),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial,"serial"),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::edges ,"edges" )
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
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_transient
{
    std::uint_least32_t parent;
    std::int_least32_t x;
    std::int_least32_t y;
    std::uint_least32_t flags;
    struct Meta
    {
        using Type = message_set_transient;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "set_transient";
        static constexpr const size_t opcode = 4;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::parent,"parent"),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::x     ,"x"     ),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::y     ,"y"     ),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::flags ,"flags" )
        );
    };
};
struct message_set_fullscreen
{
    std::uint_least32_t method;
    std::uint_least32_t framerate;
    std::uint_least32_t output;
    struct Meta
    {
        using Type = message_set_fullscreen;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "set_fullscreen";
        static constexpr const size_t opcode = 5;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::method   ,"method"   ),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::framerate,"framerate"),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::output   ,"output"   )
        );
    };
};
struct message_set_popup
{
    std::uint_least32_t seat;
    std::uint_least32_t serial;
    std::uint_least32_t parent;
    std::int_least32_t x;
    std::int_least32_t y;
    std::uint_least32_t flags;
    struct Meta
    {
        using Type = message_set_popup;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "set_popup";
        static constexpr const size_t opcode = 6;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::seat  ,"seat"  ),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial,"serial"),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::parent,"parent"),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::x     ,"x"     ),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::y     ,"y"     ),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::flags ,"flags" )
        );
    };
};
struct message_set_maximized
{
    std::uint_least32_t output;
    struct Meta
    {
        using Type = message_set_maximized;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "set_maximized";
        static constexpr const size_t opcode = 7;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::output,"output")
        );
    };
};
struct message_set_title
{
    std::string_view title;
    struct Meta
    {
        using Type = message_set_title;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "set_title";
        static constexpr const size_t opcode = 8;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::title,"title")
        );
    };
};
struct message_set_class
{
    std::string_view class_;
    struct Meta
    {
        using Type = message_set_class;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "set_class";
        static constexpr const size_t opcode = 9;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::class_,"class_")
        );
    };
};
struct message_ping
{
    std::uint_least32_t serial;
    struct Meta
    {
        using Type = message_ping;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "ping";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::serial,"serial")
        );
    };
};
struct message_configure
{
    std::uint_least32_t edges;
    std::int_least32_t width;
    std::int_least32_t height;
    struct Meta
    {
        using Type = message_configure;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "configure";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::edges ,"edges" ),
            std::make_tuple(typename TypeTagsT::wl_int{} ,&Type::width ,"width" ),
            std::make_tuple(typename TypeTagsT::wl_int{} ,&Type::height,"height")
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
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_attach
{
    std::uint_least32_t buffer;
    std::int_least32_t x;
    std::int_least32_t y;
    struct Meta
    {
        using Type = message_attach;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "attach";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::buffer,"buffer"),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::x     ,"x"     ),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::y     ,"y"     )
        );
    };
};
struct message_damage
{
    std::int_least32_t x;
    std::int_least32_t y;
    std::int_least32_t width;
    std::int_least32_t height;
    struct Meta
    {
        using Type = message_damage;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "damage";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::x     ,"x"     ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::y     ,"y"     ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::width ,"width" ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::height,"height")
        );
    };
};
struct message_frame
{
    std::uint_least32_t callback;
    struct Meta
    {
        using Type = message_frame;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "frame";
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::callback,"callback")
        );
    };
};
struct message_set_opaque_region
{
    std::uint_least32_t region;
    struct Meta
    {
        using Type = message_set_opaque_region;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "set_opaque_region";
        static constexpr const size_t opcode = 4;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::region,"region")
        );
    };
};
struct message_set_input_region
{
    std::uint_least32_t region;
    struct Meta
    {
        using Type = message_set_input_region;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "set_input_region";
        static constexpr const size_t opcode = 5;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::region,"region")
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
        static constexpr const size_t opcode = 6;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_buffer_transform
{
    std::int_least32_t transform;
    struct Meta
    {
        using Type = message_set_buffer_transform;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "set_buffer_transform";
        static constexpr const size_t opcode = 7;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::transform,"transform")
        );
    };
};
struct message_set_buffer_scale
{
    std::int_least32_t scale;
    struct Meta
    {
        using Type = message_set_buffer_scale;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "set_buffer_scale";
        static constexpr const size_t opcode = 8;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::scale,"scale")
        );
    };
};
struct message_damage_buffer
{
    std::int_least32_t x;
    std::int_least32_t y;
    std::int_least32_t width;
    std::int_least32_t height;
    struct Meta
    {
        using Type = message_damage_buffer;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "damage_buffer";
        static constexpr const size_t opcode = 9;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::x     ,"x"     ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::y     ,"y"     ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::width ,"width" ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::height,"height")
        );
    };
};
struct message_offset
{
    std::int_least32_t x;
    std::int_least32_t y;
    struct Meta
    {
        using Type = message_offset;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "offset";
        static constexpr const size_t opcode = 10;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::x,"x"),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::y,"y")
        );
    };
};
struct message_get_release
{
    std::uint_least32_t callback;
    struct Meta
    {
        using Type = message_get_release;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "get_release";
        static constexpr const size_t opcode = 11;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::callback,"callback")
        );
    };
};
struct message_enter
{
    std::uint_least32_t output;
    struct Meta
    {
        using Type = message_enter;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "enter";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::output,"output")
        );
    };
};
struct message_leave
{
    std::uint_least32_t output;
    struct Meta
    {
        using Type = message_leave;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "leave";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::output,"output")
        );
    };
};
struct message_preferred_buffer_scale
{
    std::int_least32_t factor;
    struct Meta
    {
        using Type = message_preferred_buffer_scale;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "preferred_buffer_scale";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::factor,"factor")
        );
    };
};
struct message_preferred_buffer_transform
{
    std::uint_least32_t transform;
    struct Meta
    {
        using Type = message_preferred_buffer_transform;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "preferred_buffer_transform";
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::transform,"transform")
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
    std::uint_least32_t id;
    struct Meta
    {
        using Type = message_get_pointer;
        using InterfaceTag = wl_seat;
        static constexpr char name[] = "get_pointer";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id,"id")
        );
    };
};
struct message_get_keyboard
{
    std::uint_least32_t id;
    struct Meta
    {
        using Type = message_get_keyboard;
        using InterfaceTag = wl_seat;
        static constexpr char name[] = "get_keyboard";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id,"id")
        );
    };
};
struct message_get_touch
{
    std::uint_least32_t id;
    struct Meta
    {
        using Type = message_get_touch;
        using InterfaceTag = wl_seat;
        static constexpr char name[] = "get_touch";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id,"id")
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
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_capabilities
{
    std::uint_least32_t capabilities;
    struct Meta
    {
        using Type = message_capabilities;
        using InterfaceTag = wl_seat;
        static constexpr char name[] = "capabilities";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::capabilities,"capabilities")
        );
    };
};
struct message_name
{
    std::string_view name;
    struct Meta
    {
        using Type = message_name;
        using InterfaceTag = wl_seat;
        static constexpr char name[] = "name";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::name,"name")
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
    std::uint_least32_t serial;
    std::uint_least32_t surface;
    std::int_least32_t hotspot_x;
    std::int_least32_t hotspot_y;
    struct Meta
    {
        using Type = message_set_cursor;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "set_cursor";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial   ,"serial"   ),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::surface  ,"surface"  ),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::hotspot_x,"hotspot_x"),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::hotspot_y,"hotspot_y")
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
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_enter
{
    std::uint_least32_t serial;
    std::uint_least32_t surface;
    std::uint_least32_t surface_x;
    std::uint_least32_t surface_y;
    struct Meta
    {
        using Type = message_enter;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "enter";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial   ,"serial"   ),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::surface  ,"surface"  ),
            std::make_tuple(typename TypeTagsT::wl_fixed{} ,&Type::surface_x,"surface_x"),
            std::make_tuple(typename TypeTagsT::wl_fixed{} ,&Type::surface_y,"surface_y")
        );
    };
};
struct message_leave
{
    std::uint_least32_t serial;
    std::uint_least32_t surface;
    struct Meta
    {
        using Type = message_leave;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "leave";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial ,"serial" ),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::surface,"surface")
        );
    };
};
struct message_motion
{
    std::uint_least32_t time;
    std::uint_least32_t surface_x;
    std::uint_least32_t surface_y;
    struct Meta
    {
        using Type = message_motion;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "motion";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{} ,&Type::time     ,"time"     ),
            std::make_tuple(typename TypeTagsT::wl_fixed{},&Type::surface_x,"surface_x"),
            std::make_tuple(typename TypeTagsT::wl_fixed{},&Type::surface_y,"surface_y")
        );
    };
};
struct message_button
{
    std::uint_least32_t serial;
    std::uint_least32_t time;
    std::uint_least32_t button;
    std::uint_least32_t state;
    struct Meta
    {
        using Type = message_button;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "button";
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::serial,"serial"),
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::time  ,"time"  ),
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::button,"button"),
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::state ,"state" )
        );
    };
};
struct message_axis
{
    std::uint_least32_t time;
    std::uint_least32_t axis;
    std::uint_least32_t value;
    struct Meta
    {
        using Type = message_axis;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "axis";
        static constexpr const size_t opcode = 4;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{} ,&Type::time ,"time" ),
            std::make_tuple(typename TypeTagsT::wl_uint{} ,&Type::axis ,"axis" ),
            std::make_tuple(typename TypeTagsT::wl_fixed{},&Type::value,"value")
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
        static constexpr const size_t opcode = 5;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_axis_source
{
    std::uint_least32_t axis_source;
    struct Meta
    {
        using Type = message_axis_source;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "axis_source";
        static constexpr const size_t opcode = 6;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::axis_source,"axis_source")
        );
    };
};
struct message_axis_stop
{
    std::uint_least32_t time;
    std::uint_least32_t axis;
    struct Meta
    {
        using Type = message_axis_stop;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "axis_stop";
        static constexpr const size_t opcode = 7;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::time,"time"),
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::axis,"axis")
        );
    };
};
struct message_axis_discrete
{
    std::uint_least32_t axis;
    std::int_least32_t discrete;
    struct Meta
    {
        using Type = message_axis_discrete;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "axis_discrete";
        static constexpr const size_t opcode = 8;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::axis    ,"axis"    ),
            std::make_tuple(typename TypeTagsT::wl_int{} ,&Type::discrete,"discrete")
        );
    };
};
struct message_axis_value120
{
    std::uint_least32_t axis;
    std::int_least32_t value120;
    struct Meta
    {
        using Type = message_axis_value120;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "axis_value120";
        static constexpr const size_t opcode = 9;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::axis    ,"axis"    ),
            std::make_tuple(typename TypeTagsT::wl_int{} ,&Type::value120,"value120")
        );
    };
};
struct message_axis_relative_direction
{
    std::uint_least32_t axis;
    std::uint_least32_t direction;
    struct Meta
    {
        using Type = message_axis_relative_direction;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "axis_relative_direction";
        static constexpr const size_t opcode = 10;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::axis     ,"axis"     ),
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::direction,"direction")
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_keymap
{
    std::uint_least32_t format;
    void * fd;
    std::uint_least32_t size;
    struct Meta
    {
        using Type = message_keymap;
        using InterfaceTag = wl_keyboard;
        static constexpr char name[] = "keymap";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::format,"format"),
            std::make_tuple(typename TypeTagsT::wl_fd{}  ,&Type::fd    ,"fd"    ),
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::size  ,"size"  )
        );
    };
};
struct message_enter
{
    std::uint_least32_t serial;
    std::uint_least32_t surface;
    std::span<const std::byte> keys;
    struct Meta
    {
        using Type = message_enter;
        using InterfaceTag = wl_keyboard;
        static constexpr char name[] = "enter";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial ,"serial" ),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::surface,"surface"),
            std::make_tuple(typename TypeTagsT::wl_array{} ,&Type::keys   ,"keys"   )
        );
    };
};
struct message_leave
{
    std::uint_least32_t serial;
    std::uint_least32_t surface;
    struct Meta
    {
        using Type = message_leave;
        using InterfaceTag = wl_keyboard;
        static constexpr char name[] = "leave";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial ,"serial" ),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::surface,"surface")
        );
    };
};
struct message_key
{
    std::uint_least32_t serial;
    std::uint_least32_t time;
    std::uint_least32_t key;
    std::uint_least32_t state;
    struct Meta
    {
        using Type = message_key;
        using InterfaceTag = wl_keyboard;
        static constexpr char name[] = "key";
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::serial,"serial"),
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::time  ,"time"  ),
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::key   ,"key"   ),
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::state ,"state" )
        );
    };
};
struct message_modifiers
{
    std::uint_least32_t serial;
    std::uint_least32_t mods_depressed;
    std::uint_least32_t mods_latched;
    std::uint_least32_t mods_locked;
    std::uint_least32_t group;
    struct Meta
    {
        using Type = message_modifiers;
        using InterfaceTag = wl_keyboard;
        static constexpr char name[] = "modifiers";
        static constexpr const size_t opcode = 4;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::serial        ,"serial"        ),
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::mods_depressed,"mods_depressed"),
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::mods_latched  ,"mods_latched"  ),
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::mods_locked   ,"mods_locked"   ),
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::group         ,"group"         )
        );
    };
};
struct message_repeat_info
{
    std::int_least32_t rate;
    std::int_least32_t delay;
    struct Meta
    {
        using Type = message_repeat_info;
        using InterfaceTag = wl_keyboard;
        static constexpr char name[] = "repeat_info";
        static constexpr const size_t opcode = 5;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::rate ,"rate" ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::delay,"delay")
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_down
{
    std::uint_least32_t serial;
    std::uint_least32_t time;
    std::uint_least32_t surface;
    std::int_least32_t id;
    std::uint_least32_t x;
    std::uint_least32_t y;
    struct Meta
    {
        using Type = message_down;
        using InterfaceTag = wl_touch;
        static constexpr char name[] = "down";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial ,"serial" ),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::time   ,"time"   ),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::surface,"surface"),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::id     ,"id"     ),
            std::make_tuple(typename TypeTagsT::wl_fixed{} ,&Type::x      ,"x"      ),
            std::make_tuple(typename TypeTagsT::wl_fixed{} ,&Type::y      ,"y"      )
        );
    };
};
struct message_up
{
    std::uint_least32_t serial;
    std::uint_least32_t time;
    std::int_least32_t id;
    struct Meta
    {
        using Type = message_up;
        using InterfaceTag = wl_touch;
        static constexpr char name[] = "up";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::serial,"serial"),
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::time  ,"time"  ),
            std::make_tuple(typename TypeTagsT::wl_int{} ,&Type::id    ,"id"    )
        );
    };
};
struct message_motion
{
    std::uint_least32_t time;
    std::int_least32_t id;
    std::uint_least32_t x;
    std::uint_least32_t y;
    struct Meta
    {
        using Type = message_motion;
        using InterfaceTag = wl_touch;
        static constexpr char name[] = "motion";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{} ,&Type::time,"time"),
            std::make_tuple(typename TypeTagsT::wl_int{}  ,&Type::id  ,"id"  ),
            std::make_tuple(typename TypeTagsT::wl_fixed{},&Type::x   ,"x"   ),
            std::make_tuple(typename TypeTagsT::wl_fixed{},&Type::y   ,"y"   )
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
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
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
        static constexpr const size_t opcode = 4;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_shape
{
    std::int_least32_t id;
    std::uint_least32_t major;
    std::uint_least32_t minor;
    struct Meta
    {
        using Type = message_shape;
        using InterfaceTag = wl_touch;
        static constexpr char name[] = "shape";
        static constexpr const size_t opcode = 5;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{}  ,&Type::id   ,"id"   ),
            std::make_tuple(typename TypeTagsT::wl_fixed{},&Type::major,"major"),
            std::make_tuple(typename TypeTagsT::wl_fixed{},&Type::minor,"minor")
        );
    };
};
struct message_orientation
{
    std::int_least32_t id;
    std::uint_least32_t orientation;
    struct Meta
    {
        using Type = message_orientation;
        using InterfaceTag = wl_touch;
        static constexpr char name[] = "orientation";
        static constexpr const size_t opcode = 6;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{}  ,&Type::id         ,"id"         ),
            std::make_tuple(typename TypeTagsT::wl_fixed{},&Type::orientation,"orientation")
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_geometry
{
    std::int_least32_t x;
    std::int_least32_t y;
    std::int_least32_t physical_width;
    std::int_least32_t physical_height;
    std::int_least32_t subpixel;
    std::string_view make;
    std::string_view model;
    std::int_least32_t transform;
    struct Meta
    {
        using Type = message_geometry;
        using InterfaceTag = wl_output;
        static constexpr char name[] = "geometry";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::x              ,"x"              ),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::y              ,"y"              ),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::physical_width ,"physical_width" ),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::physical_height,"physical_height"),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::subpixel       ,"subpixel"       ),
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::make           ,"make"           ),
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::model          ,"model"          ),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::transform      ,"transform"      )
        );
    };
};
struct message_mode
{
    std::uint_least32_t flags;
    std::int_least32_t width;
    std::int_least32_t height;
    std::int_least32_t refresh;
    struct Meta
    {
        using Type = message_mode;
        using InterfaceTag = wl_output;
        static constexpr char name[] = "mode";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::flags  ,"flags"  ),
            std::make_tuple(typename TypeTagsT::wl_int{} ,&Type::width  ,"width"  ),
            std::make_tuple(typename TypeTagsT::wl_int{} ,&Type::height ,"height" ),
            std::make_tuple(typename TypeTagsT::wl_int{} ,&Type::refresh,"refresh")
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
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_scale
{
    std::int_least32_t factor;
    struct Meta
    {
        using Type = message_scale;
        using InterfaceTag = wl_output;
        static constexpr char name[] = "scale";
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::factor,"factor")
        );
    };
};
struct message_name
{
    std::string_view name;
    struct Meta
    {
        using Type = message_name;
        using InterfaceTag = wl_output;
        static constexpr char name[] = "name";
        static constexpr const size_t opcode = 4;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::name,"name")
        );
    };
};
struct message_description
{
    std::string_view description;
    struct Meta
    {
        using Type = message_description;
        using InterfaceTag = wl_output;
        static constexpr char name[] = "description";
        static constexpr const size_t opcode = 5;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::description,"description")
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_add
{
    std::int_least32_t x;
    std::int_least32_t y;
    std::int_least32_t width;
    std::int_least32_t height;
    struct Meta
    {
        using Type = message_add;
        using InterfaceTag = wl_region;
        static constexpr char name[] = "add";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::x     ,"x"     ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::y     ,"y"     ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::width ,"width" ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::height,"height")
        );
    };
};
struct message_subtract
{
    std::int_least32_t x;
    std::int_least32_t y;
    std::int_least32_t width;
    std::int_least32_t height;
    struct Meta
    {
        using Type = message_subtract;
        using InterfaceTag = wl_region;
        static constexpr char name[] = "subtract";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::x     ,"x"     ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::y     ,"y"     ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::width ,"width" ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::height,"height")
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_get_subsurface
{
    std::uint_least32_t id;
    std::uint_least32_t surface;
    std::uint_least32_t parent;
    struct Meta
    {
        using Type = message_get_subsurface;
        using InterfaceTag = wl_subcompositor;
        static constexpr char name[] = "get_subsurface";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id     ,"id"     ),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::surface,"surface"),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::parent ,"parent" )
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_position
{
    std::int_least32_t x;
    std::int_least32_t y;
    struct Meta
    {
        using Type = message_set_position;
        using InterfaceTag = wl_subsurface;
        static constexpr char name[] = "set_position";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::x,"x"),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::y,"y")
        );
    };
};
struct message_place_above
{
    std::uint_least32_t sibling;
    struct Meta
    {
        using Type = message_place_above;
        using InterfaceTag = wl_subsurface;
        static constexpr char name[] = "place_above";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::sibling,"sibling")
        );
    };
};
struct message_place_below
{
    std::uint_least32_t sibling;
    struct Meta
    {
        using Type = message_place_below;
        using InterfaceTag = wl_subsurface;
        static constexpr char name[] = "place_below";
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::sibling,"sibling")
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
        static constexpr const size_t opcode = 4;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
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
        static constexpr const size_t opcode = 5;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_destroy_registry
{
    std::uint_least32_t registry;
    struct Meta
    {
        using Type = message_destroy_registry;
        using InterfaceTag = wl_fixes;
        static constexpr char name[] = "destroy_registry";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::registry,"registry")
        );
    };
};
struct message_ack_global_remove
{
    std::uint_least32_t registry;
    std::uint_least32_t name;
    struct Meta
    {
        using Type = message_ack_global_remove;
        using InterfaceTag = wl_fixes;
        static constexpr char name[] = "ack_global_remove";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::registry,"registry"),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::name    ,"name"    )
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_get_toplevel_decoration
{
    std::uint_least32_t id;
    std::uint_least32_t toplevel;
    struct Meta
    {
        using Type = message_get_toplevel_decoration;
        using InterfaceTag = zxdg_decoration_manager_v1;
        static constexpr char name[] = "get_toplevel_decoration";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id      ,"id"      ),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::toplevel,"toplevel")
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_mode
{
    std::uint_least32_t mode;
    struct Meta
    {
        using Type = message_set_mode;
        using InterfaceTag = zxdg_toplevel_decoration_v1;
        static constexpr char name[] = "set_mode";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::mode,"mode")
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
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_configure
{
    std::uint_least32_t mode;
    struct Meta
    {
        using Type = message_configure;
        using InterfaceTag = zxdg_toplevel_decoration_v1;
        static constexpr char name[] = "configure";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::mode,"mode")
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_create_positioner
{
    std::uint_least32_t id;
    struct Meta
    {
        using Type = message_create_positioner;
        using InterfaceTag = xdg_wm_base;
        static constexpr char name[] = "create_positioner";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id,"id")
        );
    };
};
struct message_get_xdg_surface
{
    std::uint_least32_t id;
    std::uint_least32_t surface;
    struct Meta
    {
        using Type = message_get_xdg_surface;
        using InterfaceTag = xdg_wm_base;
        static constexpr char name[] = "get_xdg_surface";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id     ,"id"     ),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::surface,"surface")
        );
    };
};
struct message_pong
{
    std::uint_least32_t serial;
    struct Meta
    {
        using Type = message_pong;
        using InterfaceTag = xdg_wm_base;
        static constexpr char name[] = "pong";
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::serial,"serial")
        );
    };
};
struct message_ping
{
    std::uint_least32_t serial;
    struct Meta
    {
        using Type = message_ping;
        using InterfaceTag = xdg_wm_base;
        static constexpr char name[] = "ping";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::serial,"serial")
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_size
{
    std::int_least32_t width;
    std::int_least32_t height;
    struct Meta
    {
        using Type = message_set_size;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_size";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::width ,"width" ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::height,"height")
        );
    };
};
struct message_set_anchor_rect
{
    std::int_least32_t x;
    std::int_least32_t y;
    std::int_least32_t width;
    std::int_least32_t height;
    struct Meta
    {
        using Type = message_set_anchor_rect;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_anchor_rect";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::x     ,"x"     ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::y     ,"y"     ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::width ,"width" ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::height,"height")
        );
    };
};
struct message_set_anchor
{
    std::uint_least32_t anchor;
    struct Meta
    {
        using Type = message_set_anchor;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_anchor";
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::anchor,"anchor")
        );
    };
};
struct message_set_gravity
{
    std::uint_least32_t gravity;
    struct Meta
    {
        using Type = message_set_gravity;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_gravity";
        static constexpr const size_t opcode = 4;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::gravity,"gravity")
        );
    };
};
struct message_set_constraint_adjustment
{
    std::uint_least32_t constraint_adjustment;
    struct Meta
    {
        using Type = message_set_constraint_adjustment;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_constraint_adjustment";
        static constexpr const size_t opcode = 5;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::constraint_adjustment,"constraint_adjustment")
        );
    };
};
struct message_set_offset
{
    std::int_least32_t x;
    std::int_least32_t y;
    struct Meta
    {
        using Type = message_set_offset;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_offset";
        static constexpr const size_t opcode = 6;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::x,"x"),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::y,"y")
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
        static constexpr const size_t opcode = 7;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_parent_size
{
    std::int_least32_t parent_width;
    std::int_least32_t parent_height;
    struct Meta
    {
        using Type = message_set_parent_size;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_parent_size";
        static constexpr const size_t opcode = 8;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::parent_width ,"parent_width" ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::parent_height,"parent_height")
        );
    };
};
struct message_set_parent_configure
{
    std::uint_least32_t serial;
    struct Meta
    {
        using Type = message_set_parent_configure;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_parent_configure";
        static constexpr const size_t opcode = 9;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::serial,"serial")
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_get_toplevel
{
    std::uint_least32_t id;
    struct Meta
    {
        using Type = message_get_toplevel;
        using InterfaceTag = xdg_surface;
        static constexpr char name[] = "get_toplevel";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id,"id")
        );
    };
};
struct message_get_popup
{
    std::uint_least32_t id;
    std::uint_least32_t parent;
    std::uint_least32_t positioner;
    struct Meta
    {
        using Type = message_get_popup;
        using InterfaceTag = xdg_surface;
        static constexpr char name[] = "get_popup";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_new_id{},&Type::id        ,"id"        ),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::parent    ,"parent"    ),
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::positioner,"positioner")
        );
    };
};
struct message_set_window_geometry
{
    std::int_least32_t x;
    std::int_least32_t y;
    std::int_least32_t width;
    std::int_least32_t height;
    struct Meta
    {
        using Type = message_set_window_geometry;
        using InterfaceTag = xdg_surface;
        static constexpr char name[] = "set_window_geometry";
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::x     ,"x"     ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::y     ,"y"     ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::width ,"width" ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::height,"height")
        );
    };
};
struct message_ack_configure
{
    std::uint_least32_t serial;
    struct Meta
    {
        using Type = message_ack_configure;
        using InterfaceTag = xdg_surface;
        static constexpr char name[] = "ack_configure";
        static constexpr const size_t opcode = 4;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::serial,"serial")
        );
    };
};
struct message_configure
{
    std::uint_least32_t serial;
    struct Meta
    {
        using Type = message_configure;
        using InterfaceTag = xdg_surface;
        static constexpr char name[] = "configure";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::serial,"serial")
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_parent
{
    std::uint_least32_t parent;
    struct Meta
    {
        using Type = message_set_parent;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "set_parent";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::parent,"parent")
        );
    };
};
struct message_set_title
{
    std::string_view title;
    struct Meta
    {
        using Type = message_set_title;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "set_title";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::title,"title")
        );
    };
};
struct message_set_app_id
{
    std::string_view app_id;
    struct Meta
    {
        using Type = message_set_app_id;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "set_app_id";
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_string{},&Type::app_id,"app_id")
        );
    };
};
struct message_show_window_menu
{
    std::uint_least32_t seat;
    std::uint_least32_t serial;
    std::int_least32_t x;
    std::int_least32_t y;
    struct Meta
    {
        using Type = message_show_window_menu;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "show_window_menu";
        static constexpr const size_t opcode = 4;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::seat  ,"seat"  ),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial,"serial"),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::x     ,"x"     ),
            std::make_tuple(typename TypeTagsT::wl_int{}   ,&Type::y     ,"y"     )
        );
    };
};
struct message_move
{
    std::uint_least32_t seat;
    std::uint_least32_t serial;
    struct Meta
    {
        using Type = message_move;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "move";
        static constexpr const size_t opcode = 5;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::seat  ,"seat"  ),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial,"serial")
        );
    };
};
struct message_resize
{
    std::uint_least32_t seat;
    std::uint_least32_t serial;
    std::uint_least32_t edges;
    struct Meta
    {
        using Type = message_resize;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "resize";
        static constexpr const size_t opcode = 6;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::seat  ,"seat"  ),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial,"serial"),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::edges ,"edges" )
        );
    };
};
struct message_set_max_size
{
    std::int_least32_t width;
    std::int_least32_t height;
    struct Meta
    {
        using Type = message_set_max_size;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "set_max_size";
        static constexpr const size_t opcode = 7;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::width ,"width" ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::height,"height")
        );
    };
};
struct message_set_min_size
{
    std::int_least32_t width;
    std::int_least32_t height;
    struct Meta
    {
        using Type = message_set_min_size;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "set_min_size";
        static constexpr const size_t opcode = 8;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::width ,"width" ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::height,"height")
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
        static constexpr const size_t opcode = 9;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
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
        static constexpr const size_t opcode = 10;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_set_fullscreen
{
    std::uint_least32_t output;
    struct Meta
    {
        using Type = message_set_fullscreen;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "set_fullscreen";
        static constexpr const size_t opcode = 11;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::output,"output")
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
        static constexpr const size_t opcode = 12;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
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
        static constexpr const size_t opcode = 13;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_configure
{
    std::int_least32_t width;
    std::int_least32_t height;
    std::span<const std::byte> states;
    struct Meta
    {
        using Type = message_configure;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "configure";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{}  ,&Type::width ,"width" ),
            std::make_tuple(typename TypeTagsT::wl_int{}  ,&Type::height,"height"),
            std::make_tuple(typename TypeTagsT::wl_array{},&Type::states,"states")
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
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_configure_bounds
{
    std::int_least32_t width;
    std::int_least32_t height;
    struct Meta
    {
        using Type = message_configure_bounds;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "configure_bounds";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::width ,"width" ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::height,"height")
        );
    };
};
struct message_wm_capabilities
{
    std::span<const std::byte> capabilities;
    struct Meta
    {
        using Type = message_wm_capabilities;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "wm_capabilities";
        static constexpr const size_t opcode = 3;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_array{},&Type::capabilities,"capabilities")
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
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_grab
{
    std::uint_least32_t seat;
    std::uint_least32_t serial;
    struct Meta
    {
        using Type = message_grab;
        using InterfaceTag = xdg_popup;
        static constexpr char name[] = "grab";
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::seat  ,"seat"  ),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::serial,"serial")
        );
    };
};
struct message_reposition
{
    std::uint_least32_t positioner;
    std::uint_least32_t token;
    struct Meta
    {
        using Type = message_reposition;
        using InterfaceTag = xdg_popup;
        static constexpr char name[] = "reposition";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = false;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_object{},&Type::positioner,"positioner"),
            std::make_tuple(typename TypeTagsT::wl_uint{}  ,&Type::token     ,"token"     )
        );
    };
};
struct message_configure
{
    std::int_least32_t x;
    std::int_least32_t y;
    std::int_least32_t width;
    std::int_least32_t height;
    struct Meta
    {
        using Type = message_configure;
        using InterfaceTag = xdg_popup;
        static constexpr char name[] = "configure";
        static constexpr const size_t opcode = 0;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::x     ,"x"     ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::y     ,"y"     ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::width ,"width" ),
            std::make_tuple(typename TypeTagsT::wl_int{},&Type::height,"height")
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
        static constexpr const size_t opcode = 1;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
        );
    };
};
struct message_repositioned
{
    std::uint_least32_t token;
    struct Meta
    {
        using Type = message_repositioned;
        using InterfaceTag = xdg_popup;
        static constexpr char name[] = "repositioned";
        static constexpr const size_t opcode = 2;
        static constexpr bool is_event = true;
        template<typename TypeTagsT>
        static constexpr auto args = std::make_tuple
        (
            std::make_tuple(typename TypeTagsT::wl_uint{},&Type::token,"token")
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
