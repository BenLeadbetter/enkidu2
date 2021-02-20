#pragma once

#include <type_traits>

namespace enkidu::utility {

template<typename F, typename... T>
struct is_one_of
{
    static constexpr bool value = (std::is_same_v<F, T> || ...);
};

}
