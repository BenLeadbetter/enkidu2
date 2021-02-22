#pragma once

#include <headless/app/Command.hpp>

#include <CLI11.hpp>

#include <string>

namespace enkidu::headless {

class Parser
{
public:
    Parser();
    Commands parse(std::string);

private:
    void cofigureAdd();
    void configureConnect();
    void configureRemove();
    void configureShow();
    void configureGeneral();
    Commands commands();

    CLI::App m_parser;
};

}
