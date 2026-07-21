#include "Types.hh"

#include <format>
#include <vector>

namespace wlalat
{

template <typename OIterT>
struct Formatter
{
    OIterT it;

#define WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR(TYPE)                      \
    constexpr auto operator()(const TYPE &M)                                   \
    {                                                                          \
        it = std::format_to(it, "{{");                                         \
        it = format_metadata(M);                                               \
        it = std::format_to(it, "}}");                                         \
        return it;                                                             \
    }
    WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR(CodeGen::ProtocolMetadata);
    WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR(CodeGen::InterfaceMetadata);
    WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR(CodeGen::RequestMetadata);
    WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR(CodeGen::EventMetadata);
    WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR(CodeGen::EnumMetadata);
    WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR(CodeGen::EnumEntryMetadata);
    WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR(CodeGen::ArgumentMetadata);

#undef WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR

#define WLALAT_FORMATTER_MAKE_OVERLOAD_FOR(TYPE)                               \
    constexpr auto operator()(const TYPE &M)                                   \
    {                                                                          \
        it = std::format_to(it, "{{");                                         \
        it = format(M);                                                        \
        it = std::format_to(it, "}}");                                         \
        return it;                                                             \
    }

    WLALAT_FORMATTER_MAKE_OVERLOAD_FOR(CodeGen::Protocol);
    WLALAT_FORMATTER_MAKE_OVERLOAD_FOR(CodeGen::Interface);
    WLALAT_FORMATTER_MAKE_OVERLOAD_FOR(CodeGen::Request);
    WLALAT_FORMATTER_MAKE_OVERLOAD_FOR(CodeGen::Event);
    WLALAT_FORMATTER_MAKE_OVERLOAD_FOR(CodeGen::Enum);
    WLALAT_FORMATTER_MAKE_OVERLOAD_FOR(CodeGen::EnumEntry);
    WLALAT_FORMATTER_MAKE_OVERLOAD_FOR(CodeGen::Argument);

#undef WLALAT_FORMATTER_MAKE_OVERLOAD_FOR

  private:
    template <typename T>
    constexpr auto format_array(const std::vector<T> &arr)
    {
        bool f = true;
        it = std::format_to(it, "[");
        for (auto &el : arr) {
            if (!f) {
                it = std::format_to(it, ",");
            }
            f = false;
            it = std::format_to(it, "{{");
            it = format(el);
            it = std::format_to(it, "}}");
        }
        it = std::format_to(it, "]");
        return it;
    }

    constexpr auto format(const CodeGen::Protocol &O)
    {
        it = format_metadata_key_val(O);
        it = std::format_to(it, ",\"interfaces\":");
        it = format_array(O.interfaces);
        return it;
    }

    constexpr auto format(const CodeGen::Interface &O)
    {
        it = format_metadata_key_val(O);
        it = std::format_to(it, ",\"requests\":");
        it = format_array(O.requests);
        it = std::format_to(it, ",\"events\":");
        it = format_array(O.events);
        it = std::format_to(it, ",\"enums\":");
        it = format_array(O.enums);
        return it;
    }

    constexpr auto format(const CodeGen::Enum &O)
    {
        it = format_metadata_key_val(O);
        it = std::format_to(it, ",\"entries\":");
        it = format_array(O.entries);
        return it;
    }

    constexpr auto format(const CodeGen::Request &O)
    {
        it = format_metadata_key_val(O);
        it = std::format_to(it, ",\"arguments\":");
        it = format_array(O.args);
        return it;
    }

    constexpr auto format(const CodeGen::Event &O)
    {
        it = format_metadata_key_val(O);
        it = std::format_to(it, ",\"arguments\":");
        it = format_array(O.args);
        return it;
    }

    constexpr auto format(const CodeGen::EnumEntry &O)
    {
        return format_metadata(O);
    }

    constexpr auto format(const CodeGen::Argument &O)
    {
        return format_metadata(O);
    }

    template <typename T>
    constexpr auto format_metadata_key_val(const T &V)
    {
        it = std::format_to(it, "\"metadata\":{{");
        it = format_metadata(V);
        it = std::format_to(it, "}}");
        return it;
    }

    template <typename T>
    constexpr auto format_metadata(const T &M)
    {
        auto &mappings = T::mappings;
        bool f = true;
        for (auto &m : mappings) {
            if (!f) {
                it = std::format_to(it, ",");
            }
            f = false;
            auto &op = M.*m.second;
            if (op) {
                it = std::format_to(it, "\"{}\":\"{}\"", m.first, op.value());
            } else {
                it = std::format_to(it, "\"{}\":null", m.first);
            }
        }
        return it;
    }
};

struct FormatterNoArgsBase
{
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext &ctx)
    {
        auto it = ctx.begin();
        if (it == ctx.end()) {
            return it;
        }

        if (*it != '}') {
            throw std::format_error("Invalid format args");
        }
        return it;
    }
};

template <typename T>
struct StdFormatter : FormatterNoArgsBase
{
    template <typename FmtContext>
    constexpr FmtContext::iterator format(const T &V, FmtContext &C) const
    {
        return Formatter{C.out()}(V);
    }
};

} // namespace wlalat

#define WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR(TYPE)                     \
    template <>                                                                \
    struct formatter<TYPE, char> : wlalat::StdFormatter<TYPE>                  \
    {                                                                          \
    };

namespace std
{
WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR(wlalat::CodeGen::ProtocolMetadata)
WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR(wlalat::CodeGen::InterfaceMetadata)
WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR(wlalat::CodeGen::RequestMetadata)
WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR(wlalat::CodeGen::EventMetadata)
WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR(wlalat::CodeGen::ArgumentMetadata)
WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR(wlalat::CodeGen::EnumMetadata)
WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR(wlalat::CodeGen::EnumEntryMetadata)

WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR(wlalat::CodeGen::Protocol)
WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR(wlalat::CodeGen::Interface)
WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR(wlalat::CodeGen::Request)
WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR(wlalat::CodeGen::Event)
WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR(wlalat::CodeGen::Argument)
WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR(wlalat::CodeGen::Enum)
WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR(wlalat::CodeGen::EnumEntry)

} // namespace std

#undef WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR
