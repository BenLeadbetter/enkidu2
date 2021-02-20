#pragma once

#include<model/SideEffects.hpp>

#include <udf/SideEffect.hpp>

namespace enkidu::model {

using SideEffect = udf::SideEffect<
    NodeAddedSideEffect,
    NodeRemovedSideEffect,
    NodeChangedSideEffect,
    ConnectionSideEffect,
    DisconnectionSideEffect,
    ErrorSideEffect
>;

}
