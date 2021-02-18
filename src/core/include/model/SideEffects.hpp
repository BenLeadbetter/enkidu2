#pragma once

#include <model/Connection.hpp>
#include <model/Node.hpp>

#include <string>

namespace enkidu::model {

struct NodeAddedSideEffect
{
    Node::Id id;
};

struct NodeRemovedSideEffect
{
    Node::Id id;
};

struct NodeChangedSideEffect
{
    Node::Id id;
};

struct ConnectionSideEffect
{
    Connection::Id id;
};

struct DisconnectionSideEffect
{
    Connection::Id id;
};

struct ErrorSideEffect
{
    std::string message;
};

}
