#pragma once

#include "wlalat/codegen/Types.hh"
#include "wlalat/protocol/Types.hh"

#include <vector>

namespace wlalat
{

namespace CodeGen
{

struct Converter
{
    static Protocol convert(const wlalat::Protocol::Protocol &src)
    {
        CodeGen::Protocol dst;
        convert(src.attributes, dst);
        convert(src.interfaces, dst.interfaces);
        return dst;
    }

    static Interface convert(const wlalat::Protocol::Interface &src)
    {
        CodeGen::Interface dst;
        convert(src.attributes, dst);
        convert(src.requests, dst.requests);
        convert(src.events, dst.events);
        convert(src.enums, dst.enums);
        return dst;
    }

    static Request convert(const wlalat::Protocol::Request &src)
    {
        CodeGen::Request dst;
        convert(src.attributes, dst);
        convert(src.arguments, dst.args);
        return dst;
    }

    static Event convert(const wlalat::Protocol::Event &src)
    {
        CodeGen::Event dst;
        convert(src.attributes, dst);
        convert(src.arguments, dst.args);
        return dst;
    }

    static Enum convert(const wlalat::Protocol::Enum &src)
    {
        CodeGen::Enum dst;
        convert(src.attributes, dst);
        convert(src.entries, dst.entries);
        return dst;
    }

    static Argument convert(const wlalat::Protocol::Argument &src)
    {
        CodeGen::Argument dst;
        convert(src.attributes, dst);
        return dst;
    }

    static EnumEntry convert(const wlalat::Protocol::EnumEntry &src)
    {
        CodeGen::EnumEntry dst;
        convert(src.attributes, dst);
        return dst;
    }

    template <typename SrcT, typename DstT>
    static void convert(const std::vector<SrcT> &src, std::vector<DstT> &dst)
    {
        for (auto &arg : src) {
            dst.push_back(convert(arg));
        }
    }

    template <typename MetadataType>
    static void convert(
        const std::vector<wlalat::Protocol::Attribute> &src, MetadataType &dst)
    {
        for (auto &attr : src) {
            for (auto &mapping : dst.xml_mappings) {
                if (mapping.first == attr.key) {
                    dst.*mapping.second = attr.value;
                }
            }
        }
    }
};

} // namespace CodeGen
} // namespace wlalat
