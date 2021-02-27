#pragma once

#include <immer/box.hpp>

namespace enkidu::version {

constexpr int major{0};
constexpr int minor{1};
constexpr int micro{0};

struct Number
{
    int major;
    int minor;
    int micro;
};

}
