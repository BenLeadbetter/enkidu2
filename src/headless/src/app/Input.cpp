#include <headless/app/Input.hpp>

#include <linenoise.h>

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

namespace enkidu::headless {

namespace  {

std::vector<std::string> split(const std::string& s)
{
    return {};
}

std::vector<std::string> getLine(const std::string& prompt)
{
    std::string line = linenoise(prompt.c_str());
    linenoiseHistoryAdd(line.c_str());
    //return split(line);
    return {};
}

} // namespace

Input::Input()
{
    linenoiseHistorySetMaxLen(20);
}

Command Input::get()
{
    getLine(m_prompt);
    return Command{};
}

}
