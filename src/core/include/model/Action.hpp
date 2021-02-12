#pragma once

#include <model/Actions.hpp>

#include <udf/Action.hpp>

namespace enkidu::model {

using Action = udf::Action<
    AddNodeAction,
    RemoveNodeAction,
    ConnectAction,
    DisconnectAction
>;

}
