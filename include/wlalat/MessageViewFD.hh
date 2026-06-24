#pragma once

#include "Message.hh"

#include <span>

namespace wlalat
{

template <typename FDT>
struct [[deprecated("Removing MessageView")]] MessageViewFD : MessageView
{
    MessageViewFD() = default;
    MessageViewFD(MessageView M) : MessageView{M}
    {
    }

    std::span<FDT> fds;
};

}; // namespace wlalat
