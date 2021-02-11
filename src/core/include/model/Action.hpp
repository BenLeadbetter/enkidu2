#pragma once

#include <variant>

namespace enkidu::model {

template<typename... Actions>
using Action = std::variant<Actions...>;

}
