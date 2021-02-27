#pragma once

#include <enkidu/model/Actions.hpp>
#include <enkidu/model/Model.hpp>
#include <enkidu/model/SideEffect.hpp>

#include <enkidu/udf/Reduction.hpp>

namespace enkidu::model {

using Reduction = udf::Reduction<CoreModel, SideEffect>;

Reduction reduce(CoreModel, AddNodeAction);
Reduction reduce(CoreModel, RemoveNodeAction);

Reduction reduce(CoreModel, ConnectAction);
Reduction reduce(CoreModel, DisconnectAction);

}
