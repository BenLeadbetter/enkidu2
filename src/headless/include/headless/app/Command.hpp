#pragma once

#include <headless/model/Action.hpp>

#include <enkidu/utility/flatten_variants.hpp>

#include <variant>
#include <vector>

namespace enkidu::headless {

struct ShowCommand
{
    enum Option{
        Nodes,
        Connections
    };
    Option option;
};

struct QuitCommand{};

using Command = utility::flatten_variants<
    Action,
    std::variant<
        ShowCommand,
        QuitCommand
    >
>::type;
using Commands = std::vector<Command>;

}
