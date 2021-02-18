#include <model/Reducers.hpp>
#include <model/ReducerHelpers.hpp>

#include <boost/range/algorithm/find.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <cassert>
#include <functional>
#include <utility>
#include <vector>

#include <iostream>

namespace enkidu::model {

Reduction reduce(CoreModel model, AddNodeAction)
{
    using namespace reducer_helpers;
    auto newNode = makeNode(Ports(3), Ports(3));
    return {
        update(model, model.document.nodes.set(newNode.id, std::move(newNode))),
        {NodeAddedSideEffect{newNode.id}}
    };
}

Reduction reduce(CoreModel model, RemoveNodeAction action)
{
    auto ret = Reduction{model, {}};
    const auto tocid = [](const auto& connection)
    {
        return connection.first;
    };
    const auto match = [&action](const auto& connection)
    {
        return
            connection.second.input.nodeid == action.id ||
            connection.second.output.nodeid == action.id;
    };
    auto prevConnections = boost::copy_range<std::vector<Connection::Id>>(
        model.document.connections |
        boost::adaptors::filtered(match) |
        boost::adaptors::transformed(tocid)
    );
    for (auto id : prevConnections)
    {
        auto reduction = reduce(ret.model, DisconnectAction{id});
        ret.model = reduction.model;
        ret.sideEffects.insert(
            ret.sideEffects.end(),
            reduction.sideEffects.cbegin(),
            reduction.sideEffects.cend());
    }
    ret.sideEffects.push_back(NodeRemovedSideEffect{action.id});
    return {
        reducer_helpers::update(ret.model, ret.model.document.nodes.erase(action.id)),
        ret.sideEffects
    };
}

Reduction reduce(CoreModel model, ConnectAction action)
{
    auto ret = Reduction{model, {}};
    auto connections = model.document.connections;
    const auto tocid = [](const auto& connection)
    {
        return connection.first;
    };
    const auto match = [&action](const auto& connection)
    {
        return
            connection.second.input.portid == action.input.portid ||
            connection.second.output.portid == action.output.portid;
    };
    auto prevConnections = boost::copy_range<std::vector<Connection::Id>>(
        connections |
        boost::adaptors::filtered(match) |
        boost::adaptors::transformed(tocid)
    );
    for (auto id : prevConnections)
    {
        auto reduction = reduce(ret.model, DisconnectAction{id});
        ret.model = reduction.model;
        ret.sideEffects.insert(
            ret.sideEffects.end(),
            reduction.sideEffects.cbegin(),
            reduction.sideEffects.cend());
    }
    auto newConnection = Connection{{}, action.input, action.output};
    ret.sideEffects.push_back(ConnectionSideEffect{newConnection.id});
    using namespace reducer_helpers;
    return {
        update(ret.model, ret.model.document.connections.set(newConnection.id, newConnection)),
        ret.sideEffects
    };
}

Reduction reduce(CoreModel model, DisconnectAction action)
{
    using namespace reducer_helpers;
    return {
        update(model, model.document.connections.erase(action.connectionid)),
        {DisconnectionSideEffect{action.connectionid}}
    };
}

}
