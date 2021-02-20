#include <catch2/catch.hpp>

#include <enkidu/model/Action.hpp>
#include <enkidu/model/Model.hpp>
#include <enkidu/model/Node.hpp>
#include <enkidu/model/Reducers.hpp>
#include <enkidu/model/SideEffect.hpp>

#include <enkidu/udf/Store.hpp>
#include <enkidu/udf/Reduction.hpp>

#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/find.hpp>

using namespace enkidu;

struct Reducer
{
    template<typename Action>
    udf::Reduction<model::CoreModel, model::SideEffect> reduce(model::CoreModel model, Action action)
    {
        return model::reduce(model, action);
    }
};
using Store = udf::Store<model::CoreModel, model::Action, model::SideEffect, Reducer>;

TEST_CASE("general core model test", "[actions][model][connection][reducers]")
{
    GIVEN("an empty core model with subscribers")
    {
        Store store;
        std::vector<model::Node::Id> nodeids{};
        std::vector<model::Connection::Id> connectionids{};

        const auto recordNodeId = [&nodeids](const model::NodeAddedSideEffect& se)
        {
            nodeids.push_back(se.id);
        };
        const auto removeNodeId = [&nodeids](const model::NodeRemovedSideEffect& se)
        {
            auto itr = boost::find(nodeids, se.id);
            if (itr != nodeids.cend()) nodeids.erase(itr);
        };
        const auto recordConnectionId = [&connectionids](const model::ConnectionSideEffect& se)
        {
            connectionids.push_back(se.id);
        };
        const auto removeConnectionId = [&connectionids](const model::DisconnectionSideEffect& se)
        {
            auto itr = boost::find(connectionids, se.id);
            if (itr != connectionids.cend()) connectionids.erase(itr);
        };
        int errorCount{0};
        const auto countErrors = [&errorCount](const model::ErrorSideEffect&)
        {
            ++errorCount;
        };

        store.addSubscription(recordNodeId);
        store.addSubscription(removeNodeId);
        store.addSubscription(recordConnectionId);
        store.addSubscription(removeConnectionId);
        store.addSubscription(countErrors);

        REQUIRE(store.model().document.nodes.empty());

        WHEN("dispatch an add node action")
        {
            store.dispatch(model::AddNodeAction{});

            THEN("there are no errors")
            {
                REQUIRE(!errorCount);
            }

            THEN("a node is added to the model")
            {
                REQUIRE(store.model().document.nodes.size() == 1);
            }

            THEN("the side effect subscriber is notified")
            {
                REQUIRE(!nodeids.empty());
            }
        }

        WHEN("dispatch add, remove")
        {
            store.dispatch(model::AddNodeAction{});
            REQUIRE(nodeids.size() != 0);
            store.dispatch(model::RemoveNodeAction{nodeids.front()});

            THEN("there are no errors")
            {
                REQUIRE(!errorCount);
            }

            THEN("the model is empty")
            {
                REQUIRE(store.model().document.nodes.empty());
            }
        }

        WHEN("dispatch add, add")
        {
            store.dispatch(model::AddNodeAction{});
            store.dispatch(model::AddNodeAction{});

            THEN("there are no errors")
            {
                REQUIRE(!errorCount);
            }

            THEN("the store contains two nodes")
            {
                REQUIRE(store.model().document.nodes.size() == 2);
            }
        }

        WHEN("dispatch add, add, remove(0)")
        {
            store.dispatch(model::AddNodeAction{});
            store.dispatch(model::AddNodeAction{});
            REQUIRE(nodeids.size() == 2);

            auto first = nodeids[0];
            auto second = nodeids[1];
            store.dispatch(model::RemoveNodeAction{first});

            THEN("there are no errors")
            {
                REQUIRE(!errorCount);
            }

            THEN("the second node remains")
            {
                REQUIRE(store.model().document.nodes.size() == 1);
                REQUIRE(store.model().document.nodes.begin()->first == second);
            }
        }

        WHEN("dispatch add, add, remove(1)")
        {
            store.dispatch(model::AddNodeAction{});
            store.dispatch(model::AddNodeAction{});
            REQUIRE(nodeids.size() == 2);

            auto first = nodeids[0];
            auto second = nodeids[1];
            store.dispatch(model::RemoveNodeAction{second});

            THEN("there are no errors")
            {
                REQUIRE(!errorCount);
            }

            THEN("the second node remains")
            {
                REQUIRE(store.model().document.nodes.size() == 1);
                REQUIRE(store.model().document.nodes.begin()->first == first);
            }
        }

        WHEN("dispatch add, add, connect")
        {
            store.dispatch(model::AddNodeAction{});
            store.dispatch(model::AddNodeAction{});
            REQUIRE(nodeids.size() == 2);

            auto firstNodeId = nodeids[0];
            auto secondNodeId = nodeids[1];
            auto inputid = store.model().document.nodes.find(firstNodeId)->inputs[0].id;
            auto outputid = store.model().document.nodes.find(secondNodeId)->outputs[0].id;
            store.dispatch(model::ConnectAction{inputid, outputid});

            THEN("there are no errors")
            {
                REQUIRE(!errorCount);
            }

            THEN("a connection is created")
            {
                REQUIRE(store.model().document.connections.size() == 1);
            }

            auto connection = store.model().document.connections.begin()->second;
            THEN("the connection has the requested ports")
            {
                REQUIRE(connection.input.portid == inputid);
                REQUIRE(connection.output.portid == outputid);
            }

            THEN("the connection has the requested nodes")
            {
                REQUIRE(connection.input.nodeid == nodeids[0]);
                REQUIRE(connection.output.nodeid == nodeids[1]);
            }
        }

        WHEN("dispatch an add node action and remove non-existent node")
        {
            store.dispatch(model::AddNodeAction{});
            store.dispatch(model::RemoveNodeAction{0});

            THEN("there is an error")
            {
                REQUIRE(errorCount);
            }

            THEN("the added node remains in the model")
            {
                REQUIRE(!store.model().document.nodes.empty());
            }
        }
    }
}
