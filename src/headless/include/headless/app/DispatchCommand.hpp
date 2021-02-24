#pragma once

#include <headless/app/Command.hpp>
#include <headless/model/Store.hpp>

#include <optional>

namespace enkidu::headless {

struct DispatchCommand
{
    DispatchCommand(
        Store& s,
        std::optional<int>& r) :
            store(s), ret(r) {};

    template<typename ActionCommand>
    void operator()(const ActionCommand& a)
    {
        store.dispatch(a);
    }

    void operator()(const QuitCommand&)
    {
        ret = 0;
    }

    void operator()(const ShowCommand&)
    {

    }

    Store& store;
    std::optional<int>& ret;
};

}
