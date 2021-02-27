#include <headless/app/Input.hpp>

#include <linenoise.h>

#include <boost/range/adaptor/transformed.hpp>

namespace enkidu::headless {

Input::Input()
{
    initLineNoise();
}

Commands Input::get()
{
    char* lineraw = linenoise(m_prompt.c_str());
    auto commands = m_parser.parse(lineraw);
    linenoiseHistoryAdd(lineraw);
    linenoiseFree(lineraw);
    return commands;
}

void Input::initLineNoise()
{
    linenoiseHistorySetMaxLen(20);
}

}
