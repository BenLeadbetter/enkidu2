#pragma once

#include <model/Id.hpp>

#include <immer/map.hpp>
#include <immer/vector.hpp>

namespace enkidu::model {

struct Port : public IdBase<Port> {};
using Ports = immer::vector<Port>;

struct Node : public IdBase<Node>
{
    Ports inputs;
    Ports outputs;
};

using Nodes = immer::map<Node::Id, Node>;

}
