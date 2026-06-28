#pragma once

namespace wlalat
{

template <typename VisitorT, typename MsgT>
struct [[deprecated]] ArgsIteratorWithName
{
    ArgsIteratorWithName(VisitorT &, MsgT &) = delete;
    ArgsIteratorWithName(VisitorT &, const MsgT &) = delete;
};

template <typename VisitorT, typename MsgT>
struct [[deprecated]] ArgsIterator
{
    ArgsIterator(VisitorT &V, MsgT &M)
    {
        auto UPV = [&]<typename ArgT>(ArgT &A, const char *) { V(A); };
        ArgsIteratorWithName{UPV, M};
    }
};

}; // namespace wlalat
