#include "CodeGenInfo.hh"

#include <format>

namespace wlalat
{

template <typename OIterT>
struct Formatter
{
    OIterT it;

#define WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR(TYPE)                      \
    constexpr auto operator()(const TYPE &M)                                   \
    {                                                                          \
        return format_metadata(M);                                             \
    }
    WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR(CodeGen::ProtocolMetadata);
    WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR(CodeGen::InterfaceMetadata);
    WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR(CodeGen::RequestMetadata);
    WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR(CodeGen::EventMetadata);
    WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR(CodeGen::EnumMetadata);
    WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR(CodeGen::EnumEntryMetadata);
    WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR(CodeGen::ArgumentMetadata);

#undef WLALAT_FORMATTER_MAKE_METADATA_OVERLOAD_FOR

  private:
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
} // namespace std

#undef WLALAT_MAKE_STD_FORMATTER_SPECIALISATION_FOR
