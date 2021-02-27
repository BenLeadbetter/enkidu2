#pragma once

#include <enkidu/udf/SideEffect.hpp>

#include <enkidu/utility/is_one_of.hpp>
#include <enkidu/utility/registry.hpp>

#include <functional>
#include <unordered_map>

namespace enkidu::udf {

template<typename Model>
using Diff = std::function<bool(const Model, const Model)>;
using Callback = std::function<void()>;
template<typename Model>
struct DiffSubscription
{
    Diff<Model> diff;
    Callback callback;
};

template<typename SE>
using SideEffectSubscription = std::function<void(const SE&)>;

namespace detail {

template<typename Subscription>
class SelectorBase
{
public:
    struct SubscriptionId { std::size_t id; };
    SubscriptionId addSubscription(Subscription s)
    {
        return { m_subscriptions.add(std::move(s)) };
    }
    void removeSubscription(SubscriptionId id)
    {
        m_subscriptions.remove(id.id);
    }
    void forEach (std::function<void(const Subscription&)> f)
    {
        m_subscriptions.for_each(f);
    }

private:
    utility::registry<Subscription> m_subscriptions;
};

template<typename SE>
struct SideEffectSelector : public SelectorBase<SideEffectSubscription<SE>>
{
    using SelectorBase<SideEffectSubscription<SE>>::addSubscription;
    using SelectorBase<SideEffectSubscription<SE>>::removeSubscription;
    using SelectorBase<SideEffectSubscription<SE>>::forEach;
    void select(const SE& se)
    {
        const auto exe = [&se](const auto& s)
        {
            s(se);
        };
        forEach(exe);
    }
};

template<typename Model>
struct DiffSelector : SelectorBase<DiffSubscription<Model>>
{
    using SelectorBase<DiffSubscription<Model>>::addSubscription;
    using SelectorBase<DiffSubscription<Model>>::removeSubscription;
    using SelectorBase<DiffSubscription<Model>>::forEach;
    void select(Model oldModel, Model newModel)
    {
        const auto exe = [&oldModel, &newModel](const auto& s)
        {
            if (s.diff(oldModel, newModel))
            {
                s.callback();
            }
        };
        forEach(exe);
    }
};

template<typename Model, typename SideEffect> struct SelectOverload;
template<typename Model, typename SideEffect> struct RemoveSubscriptionOverload;

} // namespace detail

template<typename Model, typename SideEffect> struct Selector;
template<typename Model, typename... SideEffects>
struct Selector<Model, SideEffect<SideEffects...>> :
        public detail::DiffSelector<Model>,
        public detail::SideEffectSelector<SideEffects>...
{
    using SubscriptionId = std::variant<
        typename detail::SideEffectSelector<SideEffects>::SubscriptionId...,
        typename detail::DiffSelector<Model>::SubscriptionId
    >;

    using detail::SideEffectSelector<SideEffects>::addSubscription...;
    template<typename SE>
    SubscriptionId addSubscription(SideEffectSubscription<SE> s)
    {
        return detail::SideEffectSelector<SE>::addSubscription(std::move(s));
    }
    using detail::DiffSelector<Model>::addSubscription;
    SubscriptionId addSubscription(DiffSubscription<Model> s)
    {
        return detail::DiffSelector<Model>::addSubscription(std::move(s));
    }

    void removeSubscription(SubscriptionId id)
    {
        std::visit(detail::RemoveSubscriptionOverload<Model, SideEffect<SideEffects...>>{*this}, id);
    }
    void select(const SideEffect<SideEffects...>& se)
    {
        std::visit(detail::SelectOverload<Model, SideEffect<SideEffects...>>{*this}, se);
    }

    using detail::DiffSelector<Model>::select;
    using detail::SideEffectSelector<SideEffects>::select...;
    using detail::SideEffectSelector<SideEffects>::removeSubscription...;
    using detail::DiffSelector<Model>::removeSubscription;
};

namespace detail {

template<typename Model, typename... SideEffects>
struct SelectOverload<Model, SideEffect<SideEffects...>>
{
    SelectOverload(Selector<Model, SideEffect<SideEffects...>>& s) : selector(s) {};
    template<typename SE>
    void operator()(const SE& se)
    {
        selector.select(se);
    }
    Selector<Model, SideEffect<SideEffects...>>& selector;
};
template<typename Model, typename... SideEffects>
struct RemoveSubscriptionOverload<Model, SideEffect<SideEffects...>>
{
    RemoveSubscriptionOverload(Selector<Model, SideEffect<SideEffects...>>& s) : selector(s) {};
    template<typename ID>
    void operator()(const ID& id)
    {
        selector.removeSubscription(id);
    }
    Selector<Model, SideEffect<SideEffects...>>& selector;
};

}

}
