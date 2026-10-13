#pragma once

#include <string>
#include <vector>

namespace wlalat
{

namespace Protocol
{

struct Attribute
{
    std::string key;
    std::string value;
};

struct Description
{
    std::vector<Attribute> attributes;
    std::vector<std::string> content_lines;
};

struct Argument
{
    std::vector<Attribute> attributes;
};

struct Request
{
    std::vector<Attribute> attributes;
    std::vector<Argument> arguments;
    Description description;
};

struct Event
{
    std::vector<Attribute> attributes;
    std::vector<Argument> arguments;
    Description description;
};

struct EnumEntry
{
    std::vector<Attribute> attributes;
    Description description;
};

struct Enum
{
    std::vector<Attribute> attributes;
    std::vector<EnumEntry> entries;
    Description description;
};

struct Interface
{
    std::vector<Attribute> attributes;
    std::vector<Request> requests;
    std::vector<Event> events;
    std::vector<Enum> enums;
    Description description;
};

struct Copyright
{
    std::vector<Attribute> attributes;
    std::vector<std::string> content_lines;
};

struct Protocol
{
    std::vector<Attribute> attributes;
    std::vector<Interface> interfaces;
    Description description;
    Copyright copyright;
};

} // namespace Protocol
} // namespace wlalat
