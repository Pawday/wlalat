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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::callback,"callback")
        );
    };
    using Type = message_sync;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::callback
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "callback"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::registry,"registry")
        );
    };
    using Type = message_get_registry;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::registry
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "registry"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::object_id,"object_id"),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::code     ,"code"     ),
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::message  ,"message"  )
        );
    };
    using Type = message_error;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::object_id,
                &Type::code,
                &Type::message
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "object_id",
                "code",
                "message"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag,
        typename PMeta::uint_tag,
        typename PMeta::string_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::id,"id")
        );
    };
    using Type = message_delete_id;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_display";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_error{},
        message_delete_id{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_sync{},
        message_get_registry{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::name        ,"name"        ),
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::id_interface,"id_interface"),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::id_version  ,"id_version"  ),
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id          ,"id"          )
        );
    };
    using Type = message_bind;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::name,
                &Type::id_interface,
                &Type::id_version,
                &Type::id
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "name",
                "id_interface",
                "id_version",
                "id"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::string_tag,
        typename PMeta::uint_tag,
        typename PMeta::new_id_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::name     ,"name"     ),
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::interface,"interface"),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::version  ,"version"  )
        );
    };
    using Type = message_global;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::name,
                &Type::interface,
                &Type::version
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "name",
                "interface",
                "version"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::string_tag,
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::name,"name")
        );
    };
    using Type = message_global_remove;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::name
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "name"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_registry";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_global{},
        message_global_remove{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_bind{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::callback_data,"callback_data")
        );
    };
    using Type = message_done;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::callback_data
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "callback_data"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_callback";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_done{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
    using Type = message_create_surface;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
    using Type = message_create_region;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag
    >{};
};
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_compositor;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 2;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_release;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_compositor";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_create_surface{},
        message_create_region{},
        message_release{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id    ,"id"    ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::offset,"offset"),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::width ,"width" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::height,"height"),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::stride,"stride"),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::format,"format")
        );
    };
    using Type = message_create_buffer;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id,
                &Type::offset,
                &Type::width,
                &Type::height,
                &Type::stride,
                &Type::format
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id",
                "offset",
                "width",
                "height",
                "stride",
                "format"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::uint_tag
    >{};
};
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = wl_shm_pool;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 1;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_destroy;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::size,"size")
        );
    };
    using Type = message_resize;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::size
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "size"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_shm_pool";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_create_buffer{},
        message_destroy{},
        message_resize{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id  ,"id"  ),
            typename PMeta::TupleMaker{}(typename PMeta::fd_tag{}    ,&Type::fd  ,"fd"  ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::size,"size")
        );
    };
    using Type = message_create_pool;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id,
                &Type::fd,
                &Type::size
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id",
                "fd",
                "size"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag,
        typename PMeta::fd_tag,
        typename PMeta::int_tag
    >{};
};
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_shm;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 1;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_release;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::format,"format")
        );
    };
    using Type = message_format;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::format
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "format"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_shm";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_format{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_create_pool{},
        message_release{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_destroy;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
};
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_buffer;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 0;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_release;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_buffer";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_release{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_destroy{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial   ,"serial"   ),
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::mime_type,"mime_type")
        );
    };
    using Type = message_accept;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial,
                &Type::mime_type
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial",
                "mime_type"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::string_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::mime_type,"mime_type"),
            typename PMeta::TupleMaker{}(typename PMeta::fd_tag{}    ,&Type::fd       ,"fd"       )
        );
    };
    using Type = message_receive;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::mime_type,
                &Type::fd
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "mime_type",
                "fd"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::string_tag,
        typename PMeta::fd_tag
    >{};
};
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = wl_data_offer;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 2;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_destroy;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
};
struct message_finish
{
    struct Meta
    {
        using Type = message_finish;
        using InterfaceTag = wl_data_offer;
        static constexpr char name[] = "finish";
        static constexpr const unsigned short opcode = 3;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_finish;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::dnd_actions     ,"dnd_actions"     ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::preferred_action,"preferred_action")
        );
    };
    using Type = message_set_actions;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::dnd_actions,
                &Type::preferred_action
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "dnd_actions",
                "preferred_action"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::mime_type,"mime_type")
        );
    };
    using Type = message_offer;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::mime_type
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "mime_type"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::string_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::source_actions,"source_actions")
        );
    };
    using Type = message_source_actions;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::source_actions
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "source_actions"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::dnd_action,"dnd_action")
        );
    };
    using Type = message_action;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::dnd_action
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "dnd_action"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_data_offer";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_offer{},
        message_source_actions{},
        message_action{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_accept{},
        message_receive{},
        message_destroy{},
        message_finish{},
        message_set_actions{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::mime_type,"mime_type")
        );
    };
    using Type = message_offer;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::mime_type
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "mime_type"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::string_tag
    >{};
};
struct message_destroy
{
    struct Meta
    {
        using Type = message_destroy;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "destroy";
        static constexpr const unsigned short opcode = 1;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_destroy;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::dnd_actions,"dnd_actions")
        );
    };
    using Type = message_set_actions;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::dnd_actions
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "dnd_actions"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::mime_type,"mime_type")
        );
    };
    using Type = message_target;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::mime_type
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "mime_type"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::string_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::mime_type,"mime_type"),
            typename PMeta::TupleMaker{}(typename PMeta::fd_tag{}    ,&Type::fd       ,"fd"       )
        );
    };
    using Type = message_send;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::mime_type,
                &Type::fd
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "mime_type",
                "fd"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::string_tag,
        typename PMeta::fd_tag
    >{};
};
struct message_cancelled
{
    struct Meta
    {
        using Type = message_cancelled;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "cancelled";
        static constexpr const unsigned short opcode = 2;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_cancelled;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
};
struct message_dnd_drop_performed
{
    struct Meta
    {
        using Type = message_dnd_drop_performed;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "dnd_drop_performed";
        static constexpr const unsigned short opcode = 3;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_dnd_drop_performed;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
};
struct message_dnd_finished
{
    struct Meta
    {
        using Type = message_dnd_finished;
        using InterfaceTag = wl_data_source;
        static constexpr char name[] = "dnd_finished";
        static constexpr const unsigned short opcode = 4;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_dnd_finished;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::dnd_action,"dnd_action")
        );
    };
    using Type = message_action;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::dnd_action
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "dnd_action"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_data_source";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_target{},
        message_send{},
        message_cancelled{},
        message_dnd_drop_performed{},
        message_dnd_finished{},
        message_action{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_offer{},
        message_destroy{},
        message_set_actions{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::source,"source"),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::origin,"origin"),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::icon  ,"icon"  ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial,"serial")
        );
    };
    using Type = message_start_drag;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::source,
                &Type::origin,
                &Type::icon,
                &Type::serial
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "source",
                "origin",
                "icon",
                "serial"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag,
        typename PMeta::object_tag,
        typename PMeta::object_tag,
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::source,"source"),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial,"serial")
        );
    };
    using Type = message_set_selection;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::source,
                &Type::serial
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "source",
                "serial"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag,
        typename PMeta::uint_tag
    >{};
};
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 2;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_release;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
    using Type = message_data_offer;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial ,"serial" ),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::surface,"surface"),
            typename PMeta::TupleMaker{}(typename PMeta::fixed_tag{} ,&Type::x      ,"x"      ),
            typename PMeta::TupleMaker{}(typename PMeta::fixed_tag{} ,&Type::y      ,"y"      ),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::id     ,"id"     )
        );
    };
    using Type = message_enter;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial,
                &Type::surface,
                &Type::x,
                &Type::y,
                &Type::id
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial",
                "surface",
                "x",
                "y",
                "id"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::object_tag,
        typename PMeta::fixed_tag,
        typename PMeta::fixed_tag,
        typename PMeta::object_tag
    >{};
};
struct message_leave
{
    struct Meta
    {
        using Type = message_leave;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "leave";
        static constexpr const unsigned short opcode = 2;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_leave;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{} ,&Type::time,"time"),
            typename PMeta::TupleMaker{}(typename PMeta::fixed_tag{},&Type::x   ,"x"   ),
            typename PMeta::TupleMaker{}(typename PMeta::fixed_tag{},&Type::y   ,"y"   )
        );
    };
    using Type = message_motion;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::time,
                &Type::x,
                &Type::y
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "time",
                "x",
                "y"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::fixed_tag,
        typename PMeta::fixed_tag
    >{};
};
struct message_drop
{
    struct Meta
    {
        using Type = message_drop;
        using InterfaceTag = wl_data_device;
        static constexpr char name[] = "drop";
        static constexpr const unsigned short opcode = 4;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_drop;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::id,"id")
        );
    };
    using Type = message_selection;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_data_device";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_data_offer{},
        message_enter{},
        message_leave{},
        message_motion{},
        message_drop{},
        message_selection{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_start_drag{},
        message_set_selection{},
        message_release{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
    using Type = message_create_data_source;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id  ,"id"  ),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::seat,"seat")
        );
    };
    using Type = message_get_data_device;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id,
                &Type::seat
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id",
                "seat"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag,
        typename PMeta::object_tag
    >{};
};
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_data_device_manager;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 2;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_release;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_data_device_manager";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_create_data_source{},
        message_get_data_device{},
        message_release{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id     ,"id"     ),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::surface,"surface")
        );
    };
    using Type = message_get_shell_surface;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id,
                &Type::surface
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id",
                "surface"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag,
        typename PMeta::object_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_shell";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_get_shell_surface{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::serial,"serial")
        );
    };
    using Type = message_pong;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::seat  ,"seat"  ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial,"serial")
        );
    };
    using Type = message_move;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::seat,
                &Type::serial
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "seat",
                "serial"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag,
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::seat  ,"seat"  ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial,"serial"),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::edges ,"edges" )
        );
    };
    using Type = message_resize;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::seat,
                &Type::serial,
                &Type::edges
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "seat",
                "serial",
                "edges"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag,
        typename PMeta::uint_tag,
        typename PMeta::uint_tag
    >{};
};
struct message_set_toplevel
{
    struct Meta
    {
        using Type = message_set_toplevel;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "set_toplevel";
        static constexpr const unsigned short opcode = 3;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_set_toplevel;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::parent,"parent"),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::x     ,"x"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::y     ,"y"     ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::flags ,"flags" )
        );
    };
    using Type = message_set_transient;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::parent,
                &Type::x,
                &Type::y,
                &Type::flags
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "parent",
                "x",
                "y",
                "flags"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::method   ,"method"   ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::framerate,"framerate"),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::output   ,"output"   )
        );
    };
    using Type = message_set_fullscreen;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::method,
                &Type::framerate,
                &Type::output
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "method",
                "framerate",
                "output"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::uint_tag,
        typename PMeta::object_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::seat  ,"seat"  ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial,"serial"),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::parent,"parent"),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::x     ,"x"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::y     ,"y"     ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::flags ,"flags" )
        );
    };
    using Type = message_set_popup;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::seat,
                &Type::serial,
                &Type::parent,
                &Type::x,
                &Type::y,
                &Type::flags
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "seat",
                "serial",
                "parent",
                "x",
                "y",
                "flags"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag,
        typename PMeta::uint_tag,
        typename PMeta::object_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::output,"output")
        );
    };
    using Type = message_set_maximized;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::output
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "output"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::title,"title")
        );
    };
    using Type = message_set_title;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::title
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "title"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::string_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::class_,"class_")
        );
    };
    using Type = message_set_class;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::class_
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "class_"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::string_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::serial,"serial")
        );
    };
    using Type = message_ping;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::edges ,"edges" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{} ,&Type::width ,"width" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{} ,&Type::height,"height")
        );
    };
    using Type = message_configure;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::edges,
                &Type::width,
                &Type::height
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "edges",
                "width",
                "height"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
};
struct message_popup_done
{
    struct Meta
    {
        using Type = message_popup_done;
        using InterfaceTag = wl_shell_surface;
        static constexpr char name[] = "popup_done";
        static constexpr const unsigned short opcode = 2;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_popup_done;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_shell_surface";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_ping{},
        message_configure{},
        message_popup_done{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_pong{},
        message_move{},
        message_resize{},
        message_set_toplevel{},
        message_set_transient{},
        message_set_fullscreen{},
        message_set_popup{},
        message_set_maximized{},
        message_set_title{},
        message_set_class{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_destroy;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::buffer,"buffer"),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::x     ,"x"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::y     ,"y"     )
        );
    };
    using Type = message_attach;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::buffer,
                &Type::x,
                &Type::y
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "buffer",
                "x",
                "y"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::x     ,"x"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::y     ,"y"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::width ,"width" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
    using Type = message_damage;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::x,
                &Type::y,
                &Type::width,
                &Type::height
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "x",
                "y",
                "width",
                "height"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::callback,"callback")
        );
    };
    using Type = message_frame;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::callback
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "callback"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::region,"region")
        );
    };
    using Type = message_set_opaque_region;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::region
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "region"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::region,"region")
        );
    };
    using Type = message_set_input_region;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::region
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "region"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag
    >{};
};
struct message_commit
{
    struct Meta
    {
        using Type = message_commit;
        using InterfaceTag = wl_surface;
        static constexpr char name[] = "commit";
        static constexpr const unsigned short opcode = 6;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_commit;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::transform,"transform")
        );
    };
    using Type = message_set_buffer_transform;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::transform
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "transform"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::scale,"scale")
        );
    };
    using Type = message_set_buffer_scale;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::scale
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "scale"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::x     ,"x"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::y     ,"y"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::width ,"width" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
    using Type = message_damage_buffer;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::x,
                &Type::y,
                &Type::width,
                &Type::height
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "x",
                "y",
                "width",
                "height"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::x,"x"),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::y,"y")
        );
    };
    using Type = message_offset;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::x,
                &Type::y
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "x",
                "y"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::callback,"callback")
        );
    };
    using Type = message_get_release;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::callback
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "callback"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::output,"output")
        );
    };
    using Type = message_enter;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::output
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "output"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::output,"output")
        );
    };
    using Type = message_leave;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::output
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "output"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::factor,"factor")
        );
    };
    using Type = message_preferred_buffer_scale;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::factor
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "factor"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::transform,"transform")
        );
    };
    using Type = message_preferred_buffer_transform;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::transform
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "transform"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_surface";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_enter{},
        message_leave{},
        message_preferred_buffer_scale{},
        message_preferred_buffer_transform{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_destroy{},
        message_attach{},
        message_damage{},
        message_frame{},
        message_set_opaque_region{},
        message_set_input_region{},
        message_commit{},
        message_set_buffer_transform{},
        message_set_buffer_scale{},
        message_damage_buffer{},
        message_offset{},
        message_get_release{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
    using Type = message_get_pointer;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
    using Type = message_get_keyboard;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
    using Type = message_get_touch;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag
    >{};
};
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_seat;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 3;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_release;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::capabilities,"capabilities")
        );
    };
    using Type = message_capabilities;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::capabilities
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "capabilities"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::name,"name")
        );
    };
    using Type = message_name;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::name
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "name"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::string_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_seat";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_capabilities{},
        message_name{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_get_pointer{},
        message_get_keyboard{},
        message_get_touch{},
        message_release{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial   ,"serial"   ),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::surface  ,"surface"  ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::hotspot_x,"hotspot_x"),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::hotspot_y,"hotspot_y")
        );
    };
    using Type = message_set_cursor;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial,
                &Type::surface,
                &Type::hotspot_x,
                &Type::hotspot_y
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial",
                "surface",
                "hotspot_x",
                "hotspot_y"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::object_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
};
struct message_release
{
    struct Meta
    {
        using Type = message_release;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "release";
        static constexpr const unsigned short opcode = 1;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_release;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial   ,"serial"   ),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::surface  ,"surface"  ),
            typename PMeta::TupleMaker{}(typename PMeta::fixed_tag{} ,&Type::surface_x,"surface_x"),
            typename PMeta::TupleMaker{}(typename PMeta::fixed_tag{} ,&Type::surface_y,"surface_y")
        );
    };
    using Type = message_enter;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial,
                &Type::surface,
                &Type::surface_x,
                &Type::surface_y
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial",
                "surface",
                "surface_x",
                "surface_y"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::object_tag,
        typename PMeta::fixed_tag,
        typename PMeta::fixed_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial ,"serial" ),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::surface,"surface")
        );
    };
    using Type = message_leave;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial,
                &Type::surface
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial",
                "surface"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::object_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{} ,&Type::time     ,"time"     ),
            typename PMeta::TupleMaker{}(typename PMeta::fixed_tag{},&Type::surface_x,"surface_x"),
            typename PMeta::TupleMaker{}(typename PMeta::fixed_tag{},&Type::surface_y,"surface_y")
        );
    };
    using Type = message_motion;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::time,
                &Type::surface_x,
                &Type::surface_y
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "time",
                "surface_x",
                "surface_y"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::fixed_tag,
        typename PMeta::fixed_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::serial,"serial"),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::time  ,"time"  ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::button,"button"),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::state ,"state" )
        );
    };
    using Type = message_button;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial,
                &Type::time,
                &Type::button,
                &Type::state
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial",
                "time",
                "button",
                "state"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::uint_tag,
        typename PMeta::uint_tag,
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{} ,&Type::time ,"time" ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{} ,&Type::axis ,"axis" ),
            typename PMeta::TupleMaker{}(typename PMeta::fixed_tag{},&Type::value,"value")
        );
    };
    using Type = message_axis;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::time,
                &Type::axis,
                &Type::value
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "time",
                "axis",
                "value"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::uint_tag,
        typename PMeta::fixed_tag
    >{};
};
struct message_frame
{
    struct Meta
    {
        using Type = message_frame;
        using InterfaceTag = wl_pointer;
        static constexpr char name[] = "frame";
        static constexpr const unsigned short opcode = 5;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_frame;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::axis_source,"axis_source")
        );
    };
    using Type = message_axis_source;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::axis_source
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "axis_source"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::time,"time"),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::axis,"axis")
        );
    };
    using Type = message_axis_stop;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::time,
                &Type::axis
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "time",
                "axis"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::axis    ,"axis"    ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{} ,&Type::discrete,"discrete")
        );
    };
    using Type = message_axis_discrete;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::axis,
                &Type::discrete
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "axis",
                "discrete"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::axis    ,"axis"    ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{} ,&Type::value120,"value120")
        );
    };
    using Type = message_axis_value120;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::axis,
                &Type::value120
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "axis",
                "value120"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::axis     ,"axis"     ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::direction,"direction")
        );
    };
    using Type = message_axis_relative_direction;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::axis,
                &Type::direction
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "axis",
                "direction"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::uint_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_pointer";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_enter{},
        message_leave{},
        message_motion{},
        message_button{},
        message_axis{},
        message_frame{},
        message_axis_source{},
        message_axis_stop{},
        message_axis_discrete{},
        message_axis_value120{},
        message_axis_relative_direction{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_set_cursor{},
        message_release{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_release;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::format,"format"),
            typename PMeta::TupleMaker{}(typename PMeta::fd_tag{}  ,&Type::fd    ,"fd"    ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::size  ,"size"  )
        );
    };
    using Type = message_keymap;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::format,
                &Type::fd,
                &Type::size
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "format",
                "fd",
                "size"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::fd_tag,
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial ,"serial" ),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::surface,"surface"),
            typename PMeta::TupleMaker{}(typename PMeta::array_tag{} ,&Type::keys   ,"keys"   )
        );
    };
    using Type = message_enter;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial,
                &Type::surface,
                &Type::keys
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial",
                "surface",
                "keys"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::object_tag,
        typename PMeta::array_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial ,"serial" ),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::surface,"surface")
        );
    };
    using Type = message_leave;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial,
                &Type::surface
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial",
                "surface"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::object_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::serial,"serial"),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::time  ,"time"  ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::key   ,"key"   ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::state ,"state" )
        );
    };
    using Type = message_key;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial,
                &Type::time,
                &Type::key,
                &Type::state
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial",
                "time",
                "key",
                "state"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::uint_tag,
        typename PMeta::uint_tag,
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::serial        ,"serial"        ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::mods_depressed,"mods_depressed"),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::mods_latched  ,"mods_latched"  ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::mods_locked   ,"mods_locked"   ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::group         ,"group"         )
        );
    };
    using Type = message_modifiers;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial,
                &Type::mods_depressed,
                &Type::mods_latched,
                &Type::mods_locked,
                &Type::group
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial",
                "mods_depressed",
                "mods_latched",
                "mods_locked",
                "group"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::uint_tag,
        typename PMeta::uint_tag,
        typename PMeta::uint_tag,
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::rate ,"rate" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::delay,"delay")
        );
    };
    using Type = message_repeat_info;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::rate,
                &Type::delay
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "rate",
                "delay"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_keyboard";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_keymap{},
        message_enter{},
        message_leave{},
        message_key{},
        message_modifiers{},
        message_repeat_info{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_release{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_release;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial ,"serial" ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::time   ,"time"   ),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::surface,"surface"),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::id     ,"id"     ),
            typename PMeta::TupleMaker{}(typename PMeta::fixed_tag{} ,&Type::x      ,"x"      ),
            typename PMeta::TupleMaker{}(typename PMeta::fixed_tag{} ,&Type::y      ,"y"      )
        );
    };
    using Type = message_down;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial,
                &Type::time,
                &Type::surface,
                &Type::id,
                &Type::x,
                &Type::y
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial",
                "time",
                "surface",
                "id",
                "x",
                "y"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::uint_tag,
        typename PMeta::object_tag,
        typename PMeta::int_tag,
        typename PMeta::fixed_tag,
        typename PMeta::fixed_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::serial,"serial"),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::time  ,"time"  ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{} ,&Type::id    ,"id"    )
        );
    };
    using Type = message_up;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial,
                &Type::time,
                &Type::id
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial",
                "time",
                "id"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::uint_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{} ,&Type::time,"time"),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}  ,&Type::id  ,"id"  ),
            typename PMeta::TupleMaker{}(typename PMeta::fixed_tag{},&Type::x   ,"x"   ),
            typename PMeta::TupleMaker{}(typename PMeta::fixed_tag{},&Type::y   ,"y"   )
        );
    };
    using Type = message_motion;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::time,
                &Type::id,
                &Type::x,
                &Type::y
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "time",
                "id",
                "x",
                "y"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::int_tag,
        typename PMeta::fixed_tag,
        typename PMeta::fixed_tag
    >{};
};
struct message_frame
{
    struct Meta
    {
        using Type = message_frame;
        using InterfaceTag = wl_touch;
        static constexpr char name[] = "frame";
        static constexpr const unsigned short opcode = 3;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_frame;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
};
struct message_cancel
{
    struct Meta
    {
        using Type = message_cancel;
        using InterfaceTag = wl_touch;
        static constexpr char name[] = "cancel";
        static constexpr const unsigned short opcode = 4;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_cancel;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}  ,&Type::id   ,"id"   ),
            typename PMeta::TupleMaker{}(typename PMeta::fixed_tag{},&Type::major,"major"),
            typename PMeta::TupleMaker{}(typename PMeta::fixed_tag{},&Type::minor,"minor")
        );
    };
    using Type = message_shape;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id,
                &Type::major,
                &Type::minor
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id",
                "major",
                "minor"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::fixed_tag,
        typename PMeta::fixed_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}  ,&Type::id         ,"id"         ),
            typename PMeta::TupleMaker{}(typename PMeta::fixed_tag{},&Type::orientation,"orientation")
        );
    };
    using Type = message_orientation;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id,
                &Type::orientation
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id",
                "orientation"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::fixed_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_touch";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_down{},
        message_up{},
        message_motion{},
        message_frame{},
        message_cancel{},
        message_shape{},
        message_orientation{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_release{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_release;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::x              ,"x"              ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::y              ,"y"              ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::physical_width ,"physical_width" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::physical_height,"physical_height"),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::subpixel       ,"subpixel"       ),
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::make           ,"make"           ),
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::model          ,"model"          ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::transform      ,"transform"      )
        );
    };
    using Type = message_geometry;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::x,
                &Type::y,
                &Type::physical_width,
                &Type::physical_height,
                &Type::subpixel,
                &Type::make,
                &Type::model,
                &Type::transform
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "x",
                "y",
                "physical_width",
                "physical_height",
                "subpixel",
                "make",
                "model",
                "transform"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::string_tag,
        typename PMeta::string_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::flags  ,"flags"  ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{} ,&Type::width  ,"width"  ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{} ,&Type::height ,"height" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{} ,&Type::refresh,"refresh")
        );
    };
    using Type = message_mode;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::flags,
                &Type::width,
                &Type::height,
                &Type::refresh
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "flags",
                "width",
                "height",
                "refresh"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
};
struct message_done
{
    struct Meta
    {
        using Type = message_done;
        using InterfaceTag = wl_output;
        static constexpr char name[] = "done";
        static constexpr const unsigned short opcode = 2;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_done;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::factor,"factor")
        );
    };
    using Type = message_scale;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::factor
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "factor"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::name,"name")
        );
    };
    using Type = message_name;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::name
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "name"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::string_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::description,"description")
        );
    };
    using Type = message_description;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::description
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "description"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::string_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_output";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_geometry{},
        message_mode{},
        message_done{},
        message_scale{},
        message_name{},
        message_description{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_release{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_destroy;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::x     ,"x"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::y     ,"y"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::width ,"width" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
    using Type = message_add;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::x,
                &Type::y,
                &Type::width,
                &Type::height
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "x",
                "y",
                "width",
                "height"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::x     ,"x"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::y     ,"y"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::width ,"width" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
    using Type = message_subtract;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::x,
                &Type::y,
                &Type::width,
                &Type::height
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "x",
                "y",
                "width",
                "height"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_region";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_destroy{},
        message_add{},
        message_subtract{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_destroy;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id     ,"id"     ),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::surface,"surface"),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::parent ,"parent" )
        );
    };
    using Type = message_get_subsurface;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id,
                &Type::surface,
                &Type::parent
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id",
                "surface",
                "parent"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag,
        typename PMeta::object_tag,
        typename PMeta::object_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_subcompositor";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_destroy{},
        message_get_subsurface{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_destroy;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::x,"x"),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::y,"y")
        );
    };
    using Type = message_set_position;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::x,
                &Type::y
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "x",
                "y"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::sibling,"sibling")
        );
    };
    using Type = message_place_above;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::sibling
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "sibling"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::sibling,"sibling")
        );
    };
    using Type = message_place_below;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::sibling
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "sibling"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag
    >{};
};
struct message_set_sync
{
    struct Meta
    {
        using Type = message_set_sync;
        using InterfaceTag = wl_subsurface;
        static constexpr char name[] = "set_sync";
        static constexpr const unsigned short opcode = 4;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_set_sync;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
};
struct message_set_desync
{
    struct Meta
    {
        using Type = message_set_desync;
        using InterfaceTag = wl_subsurface;
        static constexpr char name[] = "set_desync";
        static constexpr const unsigned short opcode = 5;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_set_desync;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_subsurface";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_destroy{},
        message_set_position{},
        message_place_above{},
        message_place_below{},
        message_set_sync{},
        message_set_desync{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_destroy;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::registry,"registry")
        );
    };
    using Type = message_destroy_registry;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::registry
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "registry"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::registry,"registry"),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::name    ,"name"    )
        );
    };
    using Type = message_ack_global_remove;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::registry,
                &Type::name
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "registry",
                "name"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag,
        typename PMeta::uint_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "wl_fixes";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_destroy{},
        message_destroy_registry{},
        message_ack_global_remove{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_destroy;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id      ,"id"      ),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::toplevel,"toplevel")
        );
    };
    using Type = message_get_toplevel_decoration;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id,
                &Type::toplevel
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id",
                "toplevel"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag,
        typename PMeta::object_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "zxdg_decoration_manager_v1";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_destroy{},
        message_get_toplevel_decoration{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_destroy;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::mode,"mode")
        );
    };
    using Type = message_set_mode;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::mode
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "mode"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
};
struct message_unset_mode
{
    struct Meta
    {
        using Type = message_unset_mode;
        using InterfaceTag = zxdg_toplevel_decoration_v1;
        static constexpr char name[] = "unset_mode";
        static constexpr const unsigned short opcode = 2;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_unset_mode;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::mode,"mode")
        );
    };
    using Type = message_configure;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::mode
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "mode"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "zxdg_toplevel_decoration_v1";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_configure{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_destroy{},
        message_set_mode{},
        message_unset_mode{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_destroy;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
    using Type = message_create_positioner;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id     ,"id"     ),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::surface,"surface")
        );
    };
    using Type = message_get_xdg_surface;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id,
                &Type::surface
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id",
                "surface"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag,
        typename PMeta::object_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::serial,"serial")
        );
    };
    using Type = message_pong;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::serial,"serial")
        );
    };
    using Type = message_ping;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "xdg_wm_base";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_ping{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_destroy{},
        message_create_positioner{},
        message_get_xdg_surface{},
        message_pong{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_destroy;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::width ,"width" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
    using Type = message_set_size;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::width,
                &Type::height
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "width",
                "height"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::x     ,"x"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::y     ,"y"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::width ,"width" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
    using Type = message_set_anchor_rect;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::x,
                &Type::y,
                &Type::width,
                &Type::height
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "x",
                "y",
                "width",
                "height"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::anchor,"anchor")
        );
    };
    using Type = message_set_anchor;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::anchor
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "anchor"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::gravity,"gravity")
        );
    };
    using Type = message_set_gravity;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::gravity
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "gravity"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::constraint_adjustment,"constraint_adjustment")
        );
    };
    using Type = message_set_constraint_adjustment;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::constraint_adjustment
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "constraint_adjustment"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::x,"x"),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::y,"y")
        );
    };
    using Type = message_set_offset;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::x,
                &Type::y
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "x",
                "y"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
};
struct message_set_reactive
{
    struct Meta
    {
        using Type = message_set_reactive;
        using InterfaceTag = xdg_positioner;
        static constexpr char name[] = "set_reactive";
        static constexpr const unsigned short opcode = 7;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_set_reactive;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::parent_width ,"parent_width" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::parent_height,"parent_height")
        );
    };
    using Type = message_set_parent_size;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::parent_width,
                &Type::parent_height
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "parent_width",
                "parent_height"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::serial,"serial")
        );
    };
    using Type = message_set_parent_configure;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "xdg_positioner";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_destroy{},
        message_set_size{},
        message_set_anchor_rect{},
        message_set_anchor{},
        message_set_gravity{},
        message_set_constraint_adjustment{},
        message_set_offset{},
        message_set_reactive{},
        message_set_parent_size{},
        message_set_parent_configure{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_destroy;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id,"id")
        );
    };
    using Type = message_get_toplevel;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::new_id_tag{},&Type::id        ,"id"        ),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::parent    ,"parent"    ),
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::positioner,"positioner")
        );
    };
    using Type = message_get_popup;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::id,
                &Type::parent,
                &Type::positioner
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "id",
                "parent",
                "positioner"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::new_id_tag,
        typename PMeta::object_tag,
        typename PMeta::object_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::x     ,"x"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::y     ,"y"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::width ,"width" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
    using Type = message_set_window_geometry;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::x,
                &Type::y,
                &Type::width,
                &Type::height
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "x",
                "y",
                "width",
                "height"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::serial,"serial")
        );
    };
    using Type = message_ack_configure;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::serial,"serial")
        );
    };
    using Type = message_configure;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::serial
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "serial"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "xdg_surface";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_configure{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_destroy{},
        message_get_toplevel{},
        message_get_popup{},
        message_set_window_geometry{},
        message_ack_configure{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_destroy;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::parent,"parent")
        );
    };
    using Type = message_set_parent;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::parent
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "parent"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::title,"title")
        );
    };
    using Type = message_set_title;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::title
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "title"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::string_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::string_tag{},&Type::app_id,"app_id")
        );
    };
    using Type = message_set_app_id;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::app_id
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "app_id"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::string_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::seat  ,"seat"  ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial,"serial"),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::x     ,"x"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}   ,&Type::y     ,"y"     )
        );
    };
    using Type = message_show_window_menu;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::seat,
                &Type::serial,
                &Type::x,
                &Type::y
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "seat",
                "serial",
                "x",
                "y"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag,
        typename PMeta::uint_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::seat  ,"seat"  ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial,"serial")
        );
    };
    using Type = message_move;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::seat,
                &Type::serial
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "seat",
                "serial"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag,
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::seat  ,"seat"  ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial,"serial"),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::edges ,"edges" )
        );
    };
    using Type = message_resize;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::seat,
                &Type::serial,
                &Type::edges
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "seat",
                "serial",
                "edges"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag,
        typename PMeta::uint_tag,
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::width ,"width" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
    using Type = message_set_max_size;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::width,
                &Type::height
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "width",
                "height"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::width ,"width" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
    using Type = message_set_min_size;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::width,
                &Type::height
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "width",
                "height"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
};
struct message_set_maximized
{
    struct Meta
    {
        using Type = message_set_maximized;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "set_maximized";
        static constexpr const unsigned short opcode = 9;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_set_maximized;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
};
struct message_unset_maximized
{
    struct Meta
    {
        using Type = message_unset_maximized;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "unset_maximized";
        static constexpr const unsigned short opcode = 10;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_unset_maximized;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::output,"output")
        );
    };
    using Type = message_set_fullscreen;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::output
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "output"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag
    >{};
};
struct message_unset_fullscreen
{
    struct Meta
    {
        using Type = message_unset_fullscreen;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "unset_fullscreen";
        static constexpr const unsigned short opcode = 12;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_unset_fullscreen;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
};
struct message_set_minimized
{
    struct Meta
    {
        using Type = message_set_minimized;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "set_minimized";
        static constexpr const unsigned short opcode = 13;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_set_minimized;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}  ,&Type::width ,"width" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{}  ,&Type::height,"height"),
            typename PMeta::TupleMaker{}(typename PMeta::array_tag{},&Type::states,"states")
        );
    };
    using Type = message_configure;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::width,
                &Type::height,
                &Type::states
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "width",
                "height",
                "states"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::array_tag
    >{};
};
struct message_close
{
    struct Meta
    {
        using Type = message_close;
        using InterfaceTag = xdg_toplevel;
        static constexpr char name[] = "close";
        static constexpr const unsigned short opcode = 1;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_close;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::width ,"width" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
    using Type = message_configure_bounds;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::width,
                &Type::height
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "width",
                "height"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::array_tag{},&Type::capabilities,"capabilities")
        );
    };
    using Type = message_wm_capabilities;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::capabilities
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "capabilities"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::array_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "xdg_toplevel";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_configure{},
        message_close{},
        message_configure_bounds{},
        message_wm_capabilities{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_destroy{},
        message_set_parent{},
        message_set_title{},
        message_set_app_id{},
        message_show_window_menu{},
        message_move{},
        message_resize{},
        message_set_max_size{},
        message_set_min_size{},
        message_set_maximized{},
        message_unset_maximized{},
        message_set_fullscreen{},
        message_unset_fullscreen{},
        message_set_minimized{}
    ));
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_destroy;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::seat  ,"seat"  ),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::serial,"serial")
        );
    };
    using Type = message_grab;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::seat,
                &Type::serial
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "seat",
                "serial"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag,
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::object_tag{},&Type::positioner,"positioner"),
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{}  ,&Type::token     ,"token"     )
        );
    };
    using Type = message_reposition;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::positioner,
                &Type::token
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "positioner",
                "token"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::object_tag,
        typename PMeta::uint_tag
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::x     ,"x"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::y     ,"y"     ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::width ,"width" ),
            typename PMeta::TupleMaker{}(typename PMeta::int_tag{},&Type::height,"height")
        );
    };
    using Type = message_configure;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::x,
                &Type::y,
                &Type::width,
                &Type::height
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "x",
                "y",
                "width",
                "height"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag,
        typename PMeta::int_tag
    >{};
};
struct message_popup_done
{
    struct Meta
    {
        using Type = message_popup_done;
        using InterfaceTag = xdg_popup;
        static constexpr char name[] = "popup_done";
        static constexpr const unsigned short opcode = 1;
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
        );
    };
    using Type = message_popup_done;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
    >{};
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
        [[deprecated]] static constexpr auto args = typename PMeta::TupleMaker{}
        (
            typename PMeta::TupleMaker{}(typename PMeta::uint_tag{},&Type::token,"token")
        );
    };
    using Type = message_repositioned;
    template <typename Visitor>
    struct VisitPointers
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                &Type::token
            );
        }
    };
    template <typename Visitor>
    struct VisitNames
    {
        Visitor &V;
        void operator()()
        {
            V
            (
                "token"
            );
        }
    };
    template <template <typename... T> class TupleLike>
    struct Tags : TupleLike
    <
        typename PMeta::uint_tag
    >{};
};
struct Meta
{
    static constexpr char name[] = "xdg_popup";
    using Events = decltype(typename PMeta::TupleMaker{}
    (
        message_configure{},
        message_popup_done{},
        message_repositioned{}
    ));
    using Requests = decltype(typename PMeta::TupleMaker{}
    (
        message_destroy{},
        message_grab{},
        message_reposition{}
    ));
};
}; // struct xdg_popup
}; // struct xdg_shell
}; // struct protocols
