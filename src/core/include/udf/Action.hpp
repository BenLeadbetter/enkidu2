#pragma once

#include <string>
#include <variant>

namespace enkidu::udf {

template<typename... Actions>
using Action = std::variant<Actions...>;

}
