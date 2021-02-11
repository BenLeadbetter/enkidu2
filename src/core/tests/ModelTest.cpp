#include <CatchConfigMain.hpp>
#include <catch2/catch.hpp>

#include <model/Action.hpp>
#include <model/Selector.hpp>
#include <model/Store.hpp>

TEST_CASE("model Test", "")
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
                selector.addSubscription(actionHappend);

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
                selector.addSubscription(becameNegative);

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
