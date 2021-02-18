#pragma once

#include <model/Actions.hpp>
#include <model/Model.hpp>
#include <model/SideEffect.hpp>

#include <udf/Reduction.hpp>

namespace enkidu::model {

using Reduction = udf::Reduction<CoreModel, SideEffect>;

Reduction reduce(CoreModel, AddNodeAction);
Reduction reduce(CoreModel, RemoveNodeAction);

Reduction reduce(CoreModel, ConnectAction);
Reduction reduce(CoreModel, DisconnectAction);

}
