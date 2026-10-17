#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace wlalat
{

template <typename MessageT, typename HandlerT>
struct PointerTagIterator
{
    explicit PointerTagIterator(HandlerT &handler) : _handler{handler}
    {
    }

    void operator()()
    {
        PointersVisitor V{_handler};
        typename MessageT::template VisitPointers<decltype(V)>{V}();
    }

  private:
    HandlerT &_handler;
    struct PointersVisitor
    {
        HandlerT &handler;

        template <typename PointersTupleT, typename TagsTupleT, size_t EndI>
        void iterate(
            PointersTupleT &,
            TagsTupleT &,
            std::integral_constant<size_t, EndI>,
            std::integral_constant<size_t, EndI>)
        {
        }

        template <
            typename PointersTupleT,
            typename TagsTupleT,
            size_t I,
            size_t EndI>
        void iterate(
            PointersTupleT &ptrs,
            TagsTupleT &tags,
            std::integral_constant<size_t, I>,
            std::integral_constant<size_t, EndI> end)
        {
            auto &pointer = std::get<I>(ptrs);
            auto &tag = std::get<I>(tags);
            handler(pointer, tag);
            using Next = std::integral_constant<size_t, I + 1>;
            iterate(ptrs, tags, Next{}, end);
        }

        template <typename... PointerTs>
        void operator()(PointerTs... ptrs)
        {
            using TagsTupleT = typename MessageT::template Tags<std::tuple>;

            using Begin = std::integral_constant<size_t, 0>;
            using End = std::integral_constant<size_t, sizeof...(ptrs)>;

            auto pointers_tuple = std::make_tuple(ptrs...);
            TagsTupleT tags_tuple{};
            iterate(pointers_tuple, tags_tuple, Begin{}, End{});
        }
    };
};
}; // namespace wlalat
