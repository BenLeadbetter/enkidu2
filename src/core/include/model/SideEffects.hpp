#pragma once

#include <model/Connection.hpp>
#include <model/Error.hpp>
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

struct ErrorSideEffect
{
    Error error;
};

}
