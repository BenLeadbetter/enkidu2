#pragma once

#include <headless/app/Command.hpp>
#include <headless/app/Parser.hpp>

#include <string>

namespace enkidu::headless {

class Input
{
public:
    Input();
    Commands get();

private:
    void initLineNoise();
    std::string m_prompt{"enkidu>> "};
    Parser m_parser;
};

}
