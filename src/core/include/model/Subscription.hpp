#pragma once

#include <functional>

namespace enkidu::model {

template<typename Model>
struct Subscription
{
    using Callback = std::function<void()>;
    using Diff = std::function<bool(Model, Model)>;
    operator bool() const { return diff && callback; }
    Diff diff;
    Callback callback;
};

}
