#pragma once

#include <model/Subscription.hpp>

#include <boost/range/algorithm/find_if.hpp>

#include <functional>
#include <vector>

namespace enkidu::model {

template<typename Model>
class Selector
{
public:
    Subscription<Model> addSubscription(
        const Subscription<Model>& subscription)
    {
        if (subscription)
        {
            m_subscriptions.push_back(subscription);
            return m_subscriptions.back();
        }
        return Subscription<Model>{};
    }
    const Subscription<Model>& addSubscription(
        typename Subscription<Model>::Diff diff,
        typename Subscription<Model>::Callback callback)
    {
        auto subscription = Subscription<Model>{diff, callback};
        return addSubscription(subscription);
    }
    bool removeSubscription(const Subscription<Model>& subscription)
    {
        auto match = [&subscription](const auto& s) { return &subscription == &s; };
        auto itr = boost::find_if(m_subscriptions, match);
        if (itr != m_subscriptions.cend())
        {
            m_subscriptions.erase(itr);
            return true;
        }
        return false;
    }

    auto process() const
    {
        return [this](auto oldModel, auto newModel)
        {
            notifySubscribers(oldModel, newModel);
        };
    }

private:
    void notifySubscribers(Model oldModel, Model newModel) const
    {
        for (const auto& subscription : m_subscriptions)
        {
            if (subscription.diff(oldModel, newModel))
            {
                subscription.callback();
            }
        }
    }
    std::vector<Subscription<Model>> m_subscriptions;
};

}
