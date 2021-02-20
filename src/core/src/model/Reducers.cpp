#include <enkidu/model/Reducers.hpp>
#include <enkidu/model/ReducerHelpers.hpp>

#include <boost/range/algorithm/find.hpp>

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

namespace  {

std::vector<Connection::Id> getConnectionsWithNode(
    Connections connections, Node::Id id)
{
    const auto match = [id](const auto& connection)
    {
        return
            connection.second.input.nodeid == id ||
            connection.second.output.nodeid == id;
    };
    return reducer_helpers::getIdsIf(connections, match);
}

std::vector<Connection::Id> getConnectionsWithPort(
    Connections connections, Port::Id id)
{
    const auto match = [id](const auto& connection)
    {
        return
            connection.second.input.portid == id ||
            connection.second.output.portid == id;
    };
    return reducer_helpers::getIdsIf(connections, match);
}

template<typename A>
Reduction reduce(Reduction reduction, const A& action)
{
    auto ret = reduce(reduction.model, action);
    ret.sideEffects.insert(
        ret.sideEffects.end(),
        reduction.sideEffects.cbegin(),
        reduction.sideEffects.cend());
    return ret;
}

} // namespace

Reduction reduce(CoreModel model, RemoveNodeAction action)
{
    auto ret = Reduction{model, {}};

    auto* node = ret.model.document.nodes.find(action.id);
    if (!node)
    {
        ret.sideEffects.push_back(ErrorSideEffect{Error::NodeNotFound});
        return ret;
    }

    auto prevConnections = getConnectionsWithNode(model.document.connections, action.id);
    for (auto id : prevConnections)
    {
        ret = reduce(ret, DisconnectAction{id});
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

    {
        using namespace reducer_helpers;
        using namespace std::placeholders;
        const auto found = [](const auto pair, auto id)
        {
            return pair.first == id;
        };
        auto inputnodes = getIdsIf(
            ret.model.document.nodes,
            std::bind(found, _1, action.input.nodeid));
        assert(inputnodes.size() < 2);
        if (inputnodes.empty())
        {
            ret.sideEffects.push_back(ErrorSideEffect{Error::PortNotFound});
            return ret;
        }

        auto outputnodes = getIdsIf(
            ret.model.document.nodes,
            std::bind(found, _1, action.output.nodeid));
        assert(outputnodes.size() < 2);
        if (inputnodes.empty())
        {
            ret.sideEffects.push_back(ErrorSideEffect{Error::PortNotFound});
            return ret;
        }
    }

    {
        auto prevInputConnections = getConnectionsWithPort(
            ret.model.document.connections, action.input.portid);
        auto prevOutputConnections = getConnectionsWithPort(
            ret.model.document.connections, action.output.portid);
        for (auto id : prevInputConnections)
        {
            ret = reduce(ret, DisconnectAction{id});
        }
        for (auto id : prevOutputConnections)
        {
            ret = reduce(ret, DisconnectAction{id});
        }
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
    auto* connection = model.document.connections.find(action.connectionid);
    if (!connection)
    {
        return {model, {ErrorSideEffect{Error::ConnectionNotFound}}};
    }
    using namespace reducer_helpers;
    return {
        update(model, model.document.connections.erase(action.connectionid)),
        {DisconnectionSideEffect{action.connectionid}}
    };
}

}
