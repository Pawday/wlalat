#pragma once

#include <unistd.h>

#include <optional>

namespace wlalat {

namespace Unix {

struct ClosableFD : private std::optional<int>
{
    ClosableFD() = default;
    ClosableFD(int val) : std::optional<int>{val}
    {
    }

    int operator()() const noexcept(false)
    {
        return value();
    }

    void close()
    {
        if (!has_value()) {
            return;
        }
        ::close(value());
    }

    ClosableFD(ClosableFD &&o) noexcept
    {
        swap(o);
    }

    ClosableFD &operator=(ClosableFD &&o) noexcept
    {
        swap(o);
        return *this;
    }

    ~ClosableFD()
    {
        close();
    }

    ClosableFD(const ClosableFD &) = default;
    ClosableFD &operator=(const ClosableFD &) = default;
};

} // namespace Unix
} // namespace wlalat
