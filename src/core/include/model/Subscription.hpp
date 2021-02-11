#pragma once

#include <functional>

namespace enkidu::model {

template<typename Model>
struct Subscription
{
    using Callback = std::function<void()>;
    using Diff = std::function<bool(Model, Model)>;
    using Id = std::size_t;

    template<typename DiffType, typename CallbackType>
    Subscription(DiffType diff, CallbackType callback) :
        diff(std::move(diff)),
        callback(std::move(callback)),
        id(std::hash<Subscription*>{}(this))
    {};

    Subscription(const Subscription& other) :
        diff(other.diff),
        callback(other.callback),
        id(std::hash<Subscription*>{}(this))
    {}

    Subscription& operator=(const Subscription& other)
    {
        diff = other.diff;
        callback = other.callback;
        return *this;
    }

    Subscription(Subscription&&) = default;
    Subscription& operator=(Subscription&&) = default;

    operator bool() const { return diff && callback; }

    Diff diff;
    Callback callback;
    const Id id;
};

}
