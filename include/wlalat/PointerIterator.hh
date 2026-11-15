#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace wlalat
{

template <typename MessageT, typename HandlerT>
struct PointerIterator
{
    explicit PointerIterator(HandlerT &handler) : _handler{handler}
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

        template <typename PointersTupleT, size_t EndI>
        void iterate(
            PointersTupleT &,
            std::integral_constant<size_t, EndI>,
            std::integral_constant<size_t, EndI>)
        {
        }

        template <typename PointersTupleT, size_t I, size_t EndI>
        void iterate(
            PointersTupleT &ptrs,
            std::integral_constant<size_t, I>,
            std::integral_constant<size_t, EndI> end)
        {
            auto &pointer = std::get<I>(ptrs);
            handler(pointer);
            using Next = std::integral_constant<size_t, I + 1>;
            iterate(ptrs, Next{}, end);
        }

        template <typename... PointerTs>
        void operator()(PointerTs... ptrs)
        {
            using Begin = std::integral_constant<size_t, 0>;
            using End = std::integral_constant<size_t, sizeof...(ptrs)>;

            auto pointers_tuple = std::make_tuple(ptrs...);
            iterate(pointers_tuple, Begin{}, End{});
        }
    };
};
}; // namespace wlalat
