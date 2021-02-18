#pragma once

#include <variant>

namespace enkidu::utility {

template<typename Variant1, typename Variant2> struct flatten_variants;
template <typename... Ts1, typename... Ts2>
struct flatten_variants<std::variant<Ts1...>, std::variant<Ts2...>>
{
    using type = std::variant<Ts1..., Ts2...>;
};

}
