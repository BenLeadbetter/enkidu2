#pragma once

#include <enkidu/app/Version.hpp>

#include <enkidu/model/Connections.hpp>
#include <enkidu/model/Node.hpp>

#include <immer/box.hpp>

#include <string>

namespace enkidu::model {

struct Document
{
    Connections connections;
    Nodes nodes;
};

struct CoreModel
{
    immer::box<const std::string> name;
    immer::box<const version::Number> version {
        version::major,
        version::minor,
        version::micro
    };
    Document document;
};

} // namespace enkdidu::core
