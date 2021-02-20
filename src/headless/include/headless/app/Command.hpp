#pragma once

#include <headless/model/Action.hpp>

#include <variant>

namespace enkidu::headless {

struct ActionCommand
{
    Action action;
};

struct ShowCommand
{
    enum Option{
        Nodes,
        Connections,
        Ports
    };
    Option option;
};

struct QuitCommand{};
struct VersionCommand{};

using Command = std::variant<
    ActionCommand,
    ShowCommand,
    QuitCommand,
    VersionCommand
>;

}
