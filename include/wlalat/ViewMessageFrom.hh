#pragma once

#include <cstddef>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

namespace wlalat
{

template <typename MessageT, typename Tags>
struct ViewMessageFrom
{
    ViewMessageFrom(const MessageT &M, std::type_identity<Tags>) : M{M}
    {
        using RefTupleT = std::remove_cvref_t<decltype(M_args)>;
        assign_all(std::make_index_sequence<std::tuple_size_v<RefTupleT>>());
    }

    using ViewType = typename MessageT::Meta::ViewType;
    ViewType value;

  private:
    static constexpr auto &M_args = MessageT::Meta::template args<Tags>;
    static constexpr auto &V_args = ViewType::Meta::template args<Tags>;

    const MessageT &M;

    template <size_t... IDX_TS>
    auto assign_all(std::index_sequence<IDX_TS...>)
    {
        (..., assign_at<IDX_TS>());
    }

    template <size_t IDX>
    auto assign_at()
    {
        auto &m_arg_mp = std::get<1>(std::get<IDX>(M_args));
        auto &view_arg_mp = std::get<1>(std::get<IDX>(V_args));

        auto &src = M.*m_arg_mp;
        const void *&dst = value.*view_arg_mp;

        dst = std::addressof(src);
    }
};

} // namespace wlalat
