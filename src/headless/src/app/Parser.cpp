#include <headless/app/Parser.hpp>

#include <algorithm>
#include <cctype>
#include <iostream>

namespace enkidu::headless {


namespace  {

std::vector<std::string> split(const std::string& s)
{
    const auto space = [](char c){ return std::isspace(c); };
    const auto nspace = [](char c){ return !std::isspace(c); };
    auto first = std::find_if(s.cbegin(), s.cend(), nspace);
    auto second = std::find_if(first, s.cend(), space);

    std::vector<std::string> ret{};
    while (first != second)
    {
        ret.push_back(std::string(first, second));
        first = find_if(second, s.cend(), nspace);
        second = find_if(first, s.cend(), space);
    }
    return ret;
}

} // namespace

Parser::Parser()
{
    m_parser.require_subcommand();
    cofigureAdd();
    configureConnect();
    configureRemove();
    configureShow();
    configureGeneral();
}

Commands Parser::parse(std::string line)
{
    for (auto& c : line) { c = std::tolower(c); }
    auto args = split(line);

    static std::vector<const char*> cargs;
    cargs.clear();
    for (const auto& s : args) cargs.push_back(s.c_str());
    try
    {
        m_parser.parse(
            static_cast<int>(cargs.size()),
            cargs.data());
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return {};
    }
    return commands();
}

void Parser::cofigureAdd()
{

}
void Parser::configureConnect()
{

}

void Parser::configureRemove()
{

}

void Parser::configureShow()
{

}

void Parser::configureGeneral()
{

}

Commands Parser::commands()
{
    return {};
}

}
