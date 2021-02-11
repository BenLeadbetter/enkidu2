#pragma once

#include <functional>
#include <variant>

namespace enkidu::model {

template<typename Model, typename Action, typename Factory>
class Reducer
{
public:
    using Reduce = std::function<Model(Model)>;
    Model operator()(Model model, Action action)
    {
        Reduce reduce = std::visit(factory, action);
        return reduce(model);
    }

private:
    Factory factory;
};

}
