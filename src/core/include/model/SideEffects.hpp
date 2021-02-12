#pragma once

#include <model/Connections.hpp>
#include <model/Node.hpp>

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

}
