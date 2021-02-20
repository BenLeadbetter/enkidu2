#pragma once

#include <model/Connection.hpp>
#include <model/Node.hpp>

namespace enkidu::model {

struct AddNodeAction {};

struct RemoveNodeAction
{
    Node::Id id;
};

struct ConnectAction
{
    Connection::Endpoint input;
    Connection::Endpoint output;
};

struct DisconnectAction
{
    Connection::Id connectionid;
};

}
