#pragma once

#include <enkidu/model/Connection.hpp>
#include <enkidu/model/Node.hpp>

namespace enkidu::model {

struct AddNodeAction {};

struct RemoveNodeAction
{
    Node::Id id;
};

struct ConnectAction
{
    Port::Id input;
    Port::Id output;
};

struct DisconnectAction
{
    Connection::Id connectionid;
};

}
