#pragma once

#include <headless/app/Command.hpp>

#include <string>

namespace enkidu::headless {

class Input
{
public:
    Input();
    Command get();

private:
    std::string m_prompt{"enkidu>> "};
};

}
