#pragma once

#include <app/Version.hpp>

#include <model/NodeMatrix.hpp>

#include <immer/box.hpp>
#include <immer/map.hpp>

namespace enkidu::model {

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

struct CoreApplication
{
    immer::box<version::Number> version {
        version::major,
        version::minor,
        version::micro
    };
    Document document_state;
};

} // namespace enkdidu::core
