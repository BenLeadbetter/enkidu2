#pragma once

#include <udf/Reduction.hpp>

#include <functional>
#include <variant>
#include <vector>

namespace enkidu::udf {

template<typename Model, typename SideEffect, typename Reducer>
struct ReducerBase
{
    using ReduceFunction = std::function<Reduction<Model, SideEffect>(Model)>;

    template<typename Action>
    Reduction<Model, SideEffect> operator()(Model model, const Action& action)
    {
        ReduceFunction reducefn = std::visit(*this, action);
        return reducefn(model);
    }
    template<typename Action>
    ReduceFunction operator()(const Action& action)
    {
        return [&action, this](auto model) { return reducer.reduce(model, action); };
    }
    Reducer reducer;
};

}
