#pragma once

#include <NodeMatrix.hpp>

#include <immer/box.hpp>
#include <immer/map.hpp>

namespace enkidu::core::model {

struct Node
{
    using Id = size_t;
    Id id;
};

struct Document
{
    immer::map<Node::Id, Node> nodes;
    immer::box<NodeMatrix> node_matrix;
};

struct Application
{
    Document document_state;
};

} // namespace enkdidu::core
