#include <CatchConfigMain.hpp>
#include <catch2/catch.hpp>

#include <udf/Action.hpp>
#include <udf/Selector.hpp>
#include <udf/Store.hpp>

TEST_CASE("model test", "[store][selector]")
{
    GIVEN("a basic model")
    {
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

        using Action = enkidu::model::Action<
            IncrementAction,
            DecrementAction,
            AddAction
        >;

        struct Factory
        {
            using Reduce = std::function<Model(Model)>;
            Reduce operator()(IncrementAction)
            {
                return [](auto model)
                {
                    return Model{++model.value};
                };
            }
            Reduce operator()(DecrementAction)
            {
                return [](auto model)
                {
                    return Model{--model.value};
                };
            }
            Reduce operator()(AddAction action)
            {
                return [action](auto model)
                {
                    return Model{model.value + action.summand};
                };
            }
        };

        using Store = enkidu::model::Store<Model, Action, Factory>;
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

        GIVEN("a selecter")
        {
            using Selector = enkidu::model::Selector<Model>;
            Selector selector;
            store.addCallback(selector.process());

            using Subscription = enkidu::model::Subscription<Model>;

            GIVEN("a trivial subscription")
            {
                int actionHappenedCount{0};
                Subscription actionHappend {
                    [](auto...) { return true; },
                    [&actionHappenedCount](){ ++actionHappenedCount; }
                };
                selector.addSubscription(std::move(actionHappend));

                WHEN("an action is dispatched")
                {
                    store.dispatch(IncrementAction{});

                    THEN("the subscriber is called back")
                    {
                        REQUIRE(actionHappenedCount == 1);
                    }
                }
            }

            GIVEN("a discerning subscription")
            {
                int becameNegativeCount{0};
                Subscription becameNegative {
                    [](auto oldModel, auto newModel) {
                        return oldModel.value >= 0 && newModel.value < 0;
                    },
                    [&becameNegativeCount](){ ++becameNegativeCount; }
                };
                selector.addSubscription(std::move(becameNegative));

                WHEN("a relevant action is dispatched")
                {
                    store.dispatch(DecrementAction{});

                    THEN("the subscriber is called back")
                    {
                        REQUIRE(becameNegativeCount == 1);
                    }
                }

                WHEN("an irrelevant action is dispatched")
                {
                    store.dispatch(IncrementAction{});

                    THEN("the subscriber is not called back")
                    {
                        REQUIRE(becameNegativeCount == 0);
                    }
                }
            }
        }
    }
}

TEST_CASE("selector test", "[selector]")
{
    GIVEN("a selector")
    {
        struct Model{};
        using Selector = enkidu::model::Selector<Model>;
        Selector selector;
        auto process = selector.process();

        using Subscription = enkidu::model::Subscription<Model>;

        WHEN("a subscription is added")
        {
            int count{0};
            Subscription subscription {
                [](auto...){ return true; },
                [&count](){ ++count; }
            };
            auto id = selector.addSubscription(std::move(subscription));

            WHEN("a new model is processed")
            {
                process(Model{}, Model{});

                THEN("the subscriber gets called back")
                {
                    REQUIRE(count == 1);
                }
            }

            WHEN("and then removed")
            {
                selector.removeSubscription(id);

                WHEN("a new model is processed")
                {
                    process(Model{}, Model{});

                    THEN("the subscriber is not notified")
                    {
                        REQUIRE(count == 0);
                    }
                }
            }
        }

        WHEN("three subscriptions are added")
        {
            auto trivialDiff = [](auto...){ return true; };

            int oneCount{0};
            Subscription one{
                trivialDiff,
                [&oneCount](){ ++oneCount; }
            };
            selector.addSubscription(std::move(one));

            int twoCount{0};
            Subscription two{
                trivialDiff,
                [&twoCount](){ ++twoCount; }
            };
            auto twoId = selector.addSubscription(std::move(two));

            int threeCount{0};
            Subscription three{
                trivialDiff,
                [&threeCount](){ ++threeCount; }
            };
            selector.addSubscription(std::move(three));

            WHEN("a new model is processed")
            {
                process(Model{}, Model{});

                THEN("all three subscriptions are called back")
                {
                    REQUIRE(oneCount == 1);
                    REQUIRE(twoCount == 1);
                    REQUIRE(threeCount == 1);
                }

                WHEN("model two is removed")
                {
                    selector.removeSubscription(twoId);

                    WHEN("a new model is processed")
                    {
                        process(Model{}, Model{});

                        THEN("one and three are called again")
                        {
                            REQUIRE(oneCount == 2);
                            REQUIRE(twoCount == 1);
                            REQUIRE(threeCount == 2);
                        }
                    }
                }
            }
        }
    }
}
