#pragma once

#include "wlalat/Types.hh"

#include <type_traits>

namespace wlalat
{

namespace Unix
{

struct WlTags
{
    using wl_int = std::type_identity<wlalat::Int>;
    using wl_uint = std::type_identity<wlalat::UInt>;
    using wl_new_id = std::type_identity<wlalat::NewID>;
    using wl_object = std::type_identity<wlalat::Object>;
    using wl_string = std::type_identity<wlalat::String>;
    using wl_array = std::type_identity<wlalat::Array>;
    using wl_fd = std::type_identity<int>;
};

} // namespace Unix
} // namespace wlalat
