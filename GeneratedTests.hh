#pragma once

#include <wlalat/Message.hh>
#include <wlalat/Parser.hh>
#include <wlalat/Types.hh>
#include <wlalat/Writer.hh>

#include <optional>

namespace Generated
{

namespace wl_registry
{

namespace message
{

struct global
{
    /* numeric name of the global object */
    wlalat::UInt name;

    /* interface implemented by the object */
    wlalat::String interface;

    /* interface version */
    wlalat::UInt version;
};

inline std::optional<global> as_global(wlalat::Message M)
{
    wlalat::Parser p{M.payload};
    global O{};

    if (!p.has<decltype(O.name)>()) return {};
    O.name = p.next<decltype(O.name)>();

    if (!p.has<decltype(O.interface)>()) return {};
    O.interface = p.next<decltype(O.interface)>();

    if (!p.has<decltype(O.version)>()) return {};
    O.version = p.next<decltype(O.version)>();

    return O;
}

template<typename OIterT>
inline void write_global(global &M, wlalat::Writer<OIterT> W)
{
    W.write(M.name);
    W.write(M.interface);
    W.write(M.version);
}

} // namespace message
} // namespace wl_registry
} // namespace Generated
