#pragma once

#include <enkidu/udf/SideEffect.hpp>

#include <vector>

namespace enkidu::udf {

template<typename Model, typename SideEffect>
struct Reduction
{
    Model model;
    using SideEffects = std::vector<SideEffect>;
    SideEffects sideEffects;
};

}
