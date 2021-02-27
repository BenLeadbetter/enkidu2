#include <CatchConfigMain.hpp>
#include <catch2/catch.hpp>

#include <enkidu/udf/Action.hpp>
#include <enkidu/udf/Reducer.hpp>
#include <enkidu/udf/Selector.hpp>
#include <enkidu/udf/SideEffect.hpp>
#include <enkidu/udf/Store.hpp>

#include <cmath>

struct Model
{
    int value{0};
};

struct IncrementAction{};
struct DecrementAction{};
struct AddAction
{
    int summand;
};

using Action = enkidu::udf::Action<
    IncrementAction,
    DecrementAction,
    AddAction
>;

struct DecreasedSideEffect{};
struct IncreasedSideEffect{};
struct ChangedSignSideEffect
{
    bool positive;
};

using SideEffect = enkidu::udf::SideEffect<
    DecreasedSideEffect,
    IncreasedSideEffect,
    ChangedSignSideEffect
>;

using Reduction = enkidu::udf::Reduction<Model, SideEffect>;

struct Reducer
{
    Reduction reduce(Model model, IncrementAction)
    {
        Reduction::SideEffects effects;
        if (model.value == -1)
        {
            effects.push_back(ChangedSignSideEffect{});
        }
        effects.push_back(IncreasedSideEffect{});
        return {
            Model{++model.value},
            effects
        };
    }
    Reduction reduce(Model model, DecrementAction)
    {
        Reduction::SideEffects effects;
        if (model.value == 0)
        {
            effects.push_back(ChangedSignSideEffect{});
        }
        effects.push_back(DecreasedSideEffect{});
        return {
            Model{--model.value},
            effects
        };
    }
    Reduction reduce(Model model, AddAction action)
    {
        Reduction::SideEffects effects;
        if (model.value * (model.value + action.summand) < 0)
        {
            effects.push_back(ChangedSignSideEffect{});
        }
        if (action.summand < 0)
        {
            effects.push_back(DecreasedSideEffect{});
        }
        if (action.summand > 0)
        {
            effects.push_back(IncreasedSideEffect{});
        }
        return {
            Model{model.value + action.summand},
            effects
        };
    }
};

using Store = enkidu::udf::Store<Model, Action, SideEffect, Reducer>;

TEST_CASE("store test - dispatching actions", "[store::dispatch]")
{
    GIVEN("a store of a basic model")
    {
        Store store;

        WHEN("dispatch basic action")
        {
            store.dispatch(IncrementAction{});

            THEN("model recieves change")
            {
                REQUIRE(store.model().value == 1);
            }
        }

        WHEN("dispatch two basic actions")
        {
            store.dispatch(IncrementAction{});
            store.dispatch(IncrementAction{});

            THEN("model recieves both changes")
            {
                REQUIRE(store.model().value == 2);
            }
        }

        WHEN("dispatch action with data")
        {
            store.dispatch(AddAction{5});

            THEN("model correctly reduces action with data")
            {
                REQUIRE(store.model().value == 5);
            }
        }

        WHEN("copy the model and dispatch an action")
        {
            auto copy = store.model();
            store.dispatch(IncrementAction{});

            THEN("the copy is unchanged")
            {
                REQUIRE(copy.value == 0);
            }

            THEN("the model recieves the change")
            {
                REQUIRE(store.model().value == 1);
            }
        }
    }
}

TEST_CASE("store test - selector", "[store::addSubscription][store::removeSubscription]")
{
    GIVEN("a store")
    {
        Store store;

        GIVEN("a side effect subscription")
        {
            int increaseCount{0};
            const auto countIncreases = [&increaseCount](const IncreasedSideEffect&)
            {
                ++increaseCount;
            };
            auto subscriptionId = store.addSubscription(countIncreases);

            WHEN("dispatch a relevant action")
            {
                store.dispatch(IncrementAction{});

                THEN("subscriber is notified")
                {
                    REQUIRE(increaseCount == 1);
                }
            }

            WHEN("dispatch a non-relevant action")
            {
                store.dispatch(AddAction{-3});

                THEN("subscriber is not notified")
                {
                    REQUIRE(increaseCount == 0);
                }
            }

            WHEN("remove subscription and dispatch relevant action")
            {
                store.removeSubscription(subscriptionId);
                store.dispatch(IncrementAction{});

                THEN("subscriber is not notified")
                {
                    REQUIRE(increaseCount == 0);
                }
            }
        }

        GIVEN("a diff subscription")
        {
            int count{0};
            const auto subscription = enkidu::udf::DiffSubscription<Model> {
                [](const auto o, const auto n){ return std::abs(o.value - n.value) > 5; },
                [&count](){++count;}
            };
            auto id = store.addSubscription(subscription);

            WHEN("dispatch a relevant action")
            {
                store.dispatch(AddAction{10});

                THEN("subscriber is notified")
                {
                    REQUIRE(count == 1);
                }
            }

            WHEN("dispatch a non-relevant action")
            {
                store.dispatch(IncrementAction{});

                THEN("subscriber is notified")
                {
                    REQUIRE(count == 0);
                }
            }

            WHEN("remove the subscription and dispatch a relevant action")
            {
                store.removeSubscription(id);
                store.dispatch(AddAction{10});

                THEN("subscriber is notified")
                {
                    REQUIRE(count == 0);
                }
            }
        }
    }
}
