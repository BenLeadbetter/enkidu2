#include <catch2/catch.hpp>

#include <enkidu/model/Action.hpp>
#include <enkidu/model/Model.hpp>
#include <enkidu/model/Node.hpp>
#include <enkidu/model/Reducers.hpp>
#include <enkidu/model/SideEffect.hpp>

#include <enkidu/udf/Action.hpp>
#include <enkidu/udf/Reducer.hpp>
#include <enkidu/udf/Store.hpp>

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
        int errorCount{0};
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
            const auto countErrors = [&errorCount](const model::ErrorSideEffect&)
            {
                ++errorCount;
            };
            store.addSubscription(std::move(nodeAddedSubscription));
            store.addSubscription(std::move(connectionSubscription));
            store.addSubscription(std::move(disconnectionSubscription));
            store.addSubscription(std::move(countErrors));
        }

        for (int i = 0; i != 5; ++i)
        {
            store.dispatch(model::AddNodeAction{});
        }
        REQUIRE(store.model().document.nodes.size() == 5);

        WHEN("node 1 is conected to node 2")
        {
            store.dispatch(model::ConnectAction{
                store.model().document.nodes.find(nodeids[0])->inputs[0].id,
                store.model().document.nodes.find(nodeids[1])->outputs[0].id});

            THEN("there are no errors")
            {
                REQUIRE(!errorCount);
            }

            THEN("the connection should be registered")
            {
                REQUIRE(store.model().document.connections.size() == 1);
            }

            WHEN("node 4 is connected to the connected output on node 2")
            {
                auto id = connectionids[0];
                store.dispatch(model::ConnectAction{
                    store.model().document.nodes.find(nodeids[3])->inputs[0].id,
                    store.model().document.nodes.find(nodeids[1])->outputs[0].id});

                THEN("there are no errors")
                {
                    REQUIRE(!errorCount);
                }

                THEN("the original connection is destroyed")
                {
                    REQUIRE(store.model().document.connections.size() == 1);
                    REQUIRE(store.model().document.connections.begin()->first != id);
                }
            }
        }
    }
}
