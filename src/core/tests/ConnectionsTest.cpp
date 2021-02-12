#include <catch2/catch.hpp>

#include <model/Action.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/Reducers.hpp>
#include <model/SideEffect.hpp>

#include <udf/Action.hpp>
#include <udf/Reducer.hpp>
#include <udf/Store.hpp>

#include <boost/range/algorithm/find.hpp>

using namespace enkidu;

using Model = model::CoreModel;
struct Reducer
{
    template<typename Action>
    udf::Reduction<Model, model::SideEffect> reduce(Model model, Action action)
    {
        return model::reduce(model, action);
    }
};
using Store = udf::Store<Model, model::Action, model::SideEffect, Reducer>;

TEST_CASE("connection management", "[connection][reducers]")
{
    GIVEN("a model conaining five nodes")
    {
        Store store;

        std::vector<model::Node::Id> nodeids;
        std::vector<model::Connection::Id> connectionids;
        {
            const auto nodeAddedSubscription = [&nodeids](const model::NodeAddedSideEffect& se)
            {
                nodeids.push_back(se.id);
            };
            const auto connectionSubscription = [&connectionids](const model::ConnectionSideEffect& se)
            {
                auto itr = boost::find(connectionids, se.id);
                if (itr == connectionids.cend()) connectionids.push_back(se.id);
            };
            const auto disconnectionSubscription = [&connectionids](const model::DisconnectionSideEffect& se)
            {
                auto itr = boost::find(connectionids, se.id);
                if (itr != connectionids.cend()) connectionids.erase(itr);
            };
            store.addSubscription(std::move(nodeAddedSubscription));
            store.addSubscription(std::move(connectionSubscription));
            store.addSubscription(std::move(disconnectionSubscription));
        }

        for (int i = 0; i != 5; ++i)
        {
            store.dispatch(model::AddNodeAction{});
        }
        REQUIRE(store.model().document.nodes.size() == 5);

        WHEN("node 1 is conected to node 2")
        {
            store.dispatch(model::ConnectAction{
                {store.model().document.nodes.find(nodeids[0])->inputs[0].id, nodeids[0]},
                {store.model().document.nodes.find(nodeids[1])->outputs[0].id, nodeids[1]}});

            THEN("the connection should be registered")
            {
                REQUIRE(store.model().document.connections.size() == 1);
            }

            WHEN("node 4 is connected to the connected output on node 2")
            {
                auto id = connectionids[0];
                store.dispatch(model::ConnectAction{
                    {store.model().document.nodes.find(nodeids[3])->inputs[0].id, nodeids[3]},
                    {store.model().document.nodes.find(nodeids[1])->outputs[0].id, nodeids[1]}});

                THEN("the original connection is destroyed")
                {
                    REQUIRE(store.model().document.connections.size() == 1);
                    REQUIRE(store.model().document.connections.begin()->first != id);
                }
            }
        }
    }
}
