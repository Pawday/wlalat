#pragma once

#include <optional>
#include <string>
#include <vector>

namespace wlalat
{

namespace CodeGen
{

using MetadataEntry = std::optional<std::string>;

struct ArgumentMetadata
{
    MetadataEntry name;
    MetadataEntry type;
    MetadataEntry interface;
    MetadataEntry summary;
    MetadataEntry allow_null;
    MetadataEntry enum_name;
};

struct RequestMetadata
{
    MetadataEntry name;
    MetadataEntry type;
    MetadataEntry since;
};

struct EventMetadata
{
    MetadataEntry name;
    MetadataEntry type;
    MetadataEntry since;
    MetadataEntry deprecated_since;
};

struct EnumEntryMetadata
{
    MetadataEntry name;
    MetadataEntry value;
    MetadataEntry summary;
    MetadataEntry since;
    MetadataEntry deprecated_since;
};

struct EnumMetadata
{
    MetadataEntry name;
    MetadataEntry since;
    MetadataEntry bitfield;
};

struct InterfaceMetadata
{
    MetadataEntry name;
    MetadataEntry version;
    MetadataEntry frozen;
};

struct ProtocolMetadata
{
    MetadataEntry name;
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
