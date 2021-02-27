#pragma once

#include<enkidu/model/SideEffects.hpp>

#include <enkidu/udf/SideEffect.hpp>

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
