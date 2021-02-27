#pragma once

#include <headless/app/Command.hpp>
#include <headless/app/Show.hpp>

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

    void operator()(const ShowCommand& c)
    {
        if (c.option == ShowCommand::Nodes)
        {
            for (auto n : store.model().document.nodes)
            {
                show(n.second);
            }
        }
        else if(c.option == ShowCommand::Connections)
        {
            for (const auto& c : store.model().document.connections)
            {
                show(c.second);
            }
        }
    }

    Store& store;
    std::optional<int>& ret;
};

}
