#pragma once

#include <variant>

namespace enkidu::udf {

template<typename... Ts>
using SideEffect = std::variant<Ts...>;

}
