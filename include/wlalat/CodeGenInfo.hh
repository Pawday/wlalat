#pragma once

#include <optional>
#include <string>
#include <vector>

namespace wlalat
{

namespace CodeGen
{

struct Argument
{
    std::optional<std::string> name;
    std::optional<std::string> type;
    std::optional<std::string> interface;
};

struct Message
{
    std::optional<std::string> name;
    std::vector<Argument> args;
};

struct Request : Message
{
};

struct Event : Message
{
};

struct EnumEntry
{
    std::optional<std::string> name;
    std::optional<std::string> value;
};

struct Enum
{
    std::optional<std::string> name;
    std::vector<EnumEntry> entries;
};

struct Interface
{
    std::optional<std::string> name;
    std::vector<Request> requests;
    std::vector<Event> events;
    std::vector<Enum> enums;
};

struct Protocol
{
    std::optional<std::string> name;
    std::vector<Interface> interfaces;
};
} // namespace CodeGen
} // namespace wlalat
