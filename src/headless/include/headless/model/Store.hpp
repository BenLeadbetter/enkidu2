#pragma once

#include <headless/model/Action.hpp>
#include <headless/model/SideEffect.hpp>

#include <enkidu/model/Action.hpp>
#include <enkidu/model/Model.hpp>
#include <enkidu/model/Reducers.hpp>

#include <enkidu/udf/Store.hpp>

namespace enkidu::headless {

using Model = model::CoreModel;
using Reduction = udf::Reduction<Model, SideEffect>;

struct Reducer
{
    template<typename Action>
    Reduction reduce(Model model, const Action& action)
    {
        return model::reduce(model, action);
    }
};

using Store = udf::Store<Model, Action, SideEffect, Reducer>;

}
