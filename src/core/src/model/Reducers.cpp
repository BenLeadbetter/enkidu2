#include <enkidu/model/Reducers.hpp>
#include <enkidu/model/ReducerHelpers.hpp>

#include <enkidu/utility/Assert.hpp>

#include <boost/range/algorithm/find.hpp>

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

Node::Id findNodeWithPort(Nodes nodes, Port::Id id)
{
    const auto matchport = [id](const auto& port) { return port.id == id; };
    const auto match = [matchport](const auto& pair)
    {
        const auto& node = pair.second;
        if (boost::find_if(node.inputs, matchport) != node.inputs.end())
        {
            return true;
        }
        if (boost::find_if(node.outputs, matchport) != node.outputs.end())
        {
            return true;
        }
    };
    auto nodeids = reducer_helpers::getIdsIf(nodes, match);
    ENKIDU_ASSERT(nodeids.size() < 2);
    if (nodeids.empty()) return 0;
    return nodeids.front();
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

    auto inputnodeid = findNodeWithPort(ret.model.document.nodes, action.input);
    auto outputnodeid = findNodeWithPort(ret.model.document.nodes, action.output);

    if (!inputnodeid || !outputnodeid)
    {
        ret.sideEffects.push_back(ErrorSideEffect{Error::PortNotFound});
        return ret;
    }

    {
        auto prevInputConnections = getConnectionsWithPort(
            ret.model.document.connections, action.input);
        auto prevOutputConnections = getConnectionsWithPort(
            ret.model.document.connections, action.output);
        for (auto id : prevInputConnections)
        {
            ret = reduce(ret, DisconnectAction{id});
        }
        for (auto id : prevOutputConnections)
        {
            ret = reduce(ret, DisconnectAction{id});
        }
    }

    auto newConnection = Connection{
        {},
        {action.input, inputnodeid},
        {action.output, outputnodeid}
    };
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
