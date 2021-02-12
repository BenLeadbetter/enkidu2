#pragma once

#include <boost/range/algorithm/find_if.hpp>

#include <cstddef>
#include <set>
#include <type_traits>

namespace enkidu::model {

template<typename T>
struct IdBase
{
    using Id = std::size_t;
    IdBase(Id i) : id(i){};
    IdBase() : id(++sm_count) {};
    size_t id;

private:
    static std::size_t sm_count;
};

template<typename T>
std::size_t IdBase<T>::sm_count = 0;

template<typename T>
bool operator==(
    const std::enable_if<std::is_base_of<IdBase<T>, T>::type, T>& l,
    const std::enable_if<std::is_base_of<IdBase<T>, T>::type, T>& r)
{
    return l.id == r.id;
}

}
