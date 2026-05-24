#pragma once

#include <wlalat/Message.hh>
#include <wlalat/Parser.hh>
#include <wlalat/Types.hh>

#include <optional>

namespace Generated {

struct wl_registry_message_global
{
    /* numeric name of the global object */
    wlalat::UInt name;

    /* interface implemented by the object */
    wlalat::String interface;

    /* interface version */
    wlalat::UInt version;

    static auto parse(
        wlalat::Message M) -> std::optional<wl_registry_message_global>
    {
        wlalat::Parser p{M.payload};
        wl_registry_message_global O{};

        using name_t = decltype(O.name);
        if (!p.has<name_t>()) { return {}; }
        O.name = p.next<name_t>();

        using interface_t = decltype(O.interface);
        if(!p.has<interface_t>()) { return {}; }
        O.interface = p.next<interface_t>();

        using version_t = decltype(O.version);
        if(!p.has<version_t>()) { return {}; };
        O.version = p.next<version_t>();

        return O;
    }
};

}; // namespace Generated
