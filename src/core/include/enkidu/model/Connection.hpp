#pragma once

#include <enkidu/model/Node.hpp>
#include <enkidu/model/Id.hpp>

#include <immer/map.hpp>
#include <immer/vector.hpp>

namespace enkidu::model {

struct Connection : public IdBase<Connection>
{
    struct Endpoint
    {
        Port::Id portid;
        Port::Id nodeid;
    };
    Endpoint input;
    Endpoint output;
};

using Connections = immer::map<Connection::Id, Connection>;

}
