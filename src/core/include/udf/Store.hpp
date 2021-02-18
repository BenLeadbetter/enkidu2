#pragma once

#include <udf/Reducer.hpp>
#include <udf/Selector.hpp>

namespace enkidu::udf {

template<
    typename Model,
    typename Action,
    typename SideEffect,
    typename Reducer
>
class Store
{
public:
    const Model model() const
    {
        return m_model;
    }
    void dispatch(Action action)
    {
        auto oldModel = m_model;
        auto reduction = m_reducer(m_model, action);
        m_model = std::move(reduction.model);
        for (const auto& se : reduction.sideEffects)
        {
            m_selector.select(se);
        }
        m_selector.select(oldModel, m_model);
    }

    using SubscriptionId = typename Selector<Model, SideEffect>::SubscriptionId;
    template<typename Subscription>
    SubscriptionId addSubscription(Subscription s)
    {
        return m_selector.addSubscription(std::move(s));
    }
    void removeSubscription(SubscriptionId id)
    {
        m_selector.removeSubscription(id);
    }

private:
    Model m_model;
    ReducerBase<Model, SideEffect, Reducer> m_reducer;
    Selector<Model, SideEffect> m_selector;
};

}
