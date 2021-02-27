#pragma once

#include <enkidu/model/Actions.hpp>

#include <enkidu/udf/Action.hpp>

namespace enkidu::model {

using Action = udf::Action<
    AddNodeAction,
    RemoveNodeAction,
    ConnectAction,
    DisconnectAction
>;

}
