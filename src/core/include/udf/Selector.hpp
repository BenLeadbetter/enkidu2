#pragma once

#include <udf/Subscription.hpp>

#include <boost/range/algorithm/find_if.hpp>

#include <functional>
#include <vector>

namespace enkidu::model {

template<typename Model>
class Selector
{
public:
    typename Subscription<Model>::Id addSubscription(
        const Subscription<Model>& subscription)
    {
        if (subscription)
        {
            m_subscriptions.push_back(subscription);
            return m_subscriptions.back().id;
        }
        return 0;
    }
    typename Subscription<Model>::Id addSubscription(
        typename Subscription<Model>::Diff diff,
        typename Subscription<Model>::Callback callback)
    {
        auto subscription = Subscription<Model>{diff, callback};
        return addSubscription(std::move(subscription));
    }
    bool removeSubscription(typename Subscription<Model>::Id id)
    {
        auto match = [id](const auto& s) { return id == s.id; };
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
        return [this](const auto oldModel, const auto newModel)
        {
            notifySubscribers(oldModel, newModel);
        };
    }

private:
    void notifySubscribers(const Model oldModel, const Model newModel) const
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
