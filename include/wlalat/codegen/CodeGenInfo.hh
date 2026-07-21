#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace wlalat
{

namespace CodeGen
{

using MetadataEntry = std::optional<std::string>;

template <typename MetadataType>
using MetadataStringMappingType =
    std::pair<const char *, MetadataEntry MetadataType::*>;

struct ArgumentMetadata
{
    using Type = ArgumentMetadata;

    MetadataEntry name;
    MetadataEntry type;
    MetadataEntry interface;
    MetadataEntry summary;
    MetadataEntry allow_null;
    MetadataEntry enum_name;

    static constexpr MetadataStringMappingType<Type> mappings[]{
        {"name", &Type::name},
        {"type", &Type::type},
        {"interface", &Type::interface},
        {"summary", &Type::summary},
        {"allow_null", &Type::allow_null},
        {"enum_name", &Type::enum_name},
    };
}; // namespace CodeGen

struct RequestMetadata
{
    using Type = RequestMetadata;

    MetadataEntry name;
    MetadataEntry type;
    MetadataEntry since;

    static constexpr MetadataStringMappingType<Type> mappings[]{
        {"name", &Type::name},
        {"type", &Type::type},
        {"since", &Type::since},
    };
};

struct EventMetadata
{
    using Type = EventMetadata;

    MetadataEntry name;
    MetadataEntry type;
    MetadataEntry since;
    MetadataEntry deprecated_since;

    static constexpr MetadataStringMappingType<Type> mappings[]{
        {"name", &Type::name},
        {"type", &Type::type},
        {"since", &Type::since},
        {"deprecated_since", &Type::deprecated_since},
    };
};

struct EnumEntryMetadata
{
    using Type = EnumEntryMetadata;

    MetadataEntry name;
    MetadataEntry value;
    MetadataEntry summary;
    MetadataEntry since;
    MetadataEntry deprecated_since;

    static constexpr MetadataStringMappingType<Type> mappings[]{
        {"name", &Type::name},
        {"value", &Type::value},
        {"summary", &Type::summary},
        {"since", &Type::since},
        {"deprecated_since", &Type::deprecated_since},
    };
};

struct EnumMetadata
{
    using Type = EnumMetadata;

    MetadataEntry name;
    MetadataEntry since;
    MetadataEntry bitfield;

    static constexpr MetadataStringMappingType<Type> mappings[]{
        {"name", &Type::name},
        {"since", &Type::since},
        {"bitfield", &Type::bitfield},
    };
};

struct InterfaceMetadata
{
    using Type = InterfaceMetadata;

    MetadataEntry name;
    MetadataEntry version;
    MetadataEntry frozen;

    static constexpr MetadataStringMappingType<Type> mappings[]{
        {"name", &Type::name},
        {"version", &Type::version},
        {"frozen", &Type::frozen},
    };
};

struct ProtocolMetadata
{
    using Type = ProtocolMetadata;

    MetadataEntry name;

    static constexpr MetadataStringMappingType<Type> mappings[]{
        {"name", &Type::name},
    };
};

struct Argument : ArgumentMetadata
{
};

struct Request : RequestMetadata
{
    std::vector<Argument> args;
};

struct Event : EventMetadata
{
    std::vector<Argument> args;
};

struct EnumEntry : EnumEntryMetadata
{
};

struct Enum : EnumMetadata
{
    std::vector<EnumEntry> entries;
};

struct Interface : InterfaceMetadata
{
    std::vector<Request> requests;
    std::vector<Event> events;
    std::vector<Enum> enums;
};

struct Protocol : ProtocolMetadata
{
    std::vector<Interface> interfaces;
};
} // namespace CodeGen
} // namespace wlalat
