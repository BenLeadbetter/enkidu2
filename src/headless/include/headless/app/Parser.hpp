#pragma once

#include <headless/app/Command.hpp>

#include <CLI11.hpp>

#include <iostream>
#include <string>

namespace enkidu::headless {

class Parser
{
public:
    Parser(std::ostream& err = std::cerr);
    Commands parse(std::string);

private:
    void cofigureAdd();
    void configureConnect();
    void configureRemove();
    void configureShow();
    void configureGeneral();
    void reset();

    Commands m_commands;
    CLI::App m_parser;
    std::ostream& m_err;
};

}
