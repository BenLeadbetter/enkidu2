#include <headless/app/Parser.hpp>

#include <boost/range/algorithm/find_if.hpp>

#include <algorithm>
#include <cctype>
#include <cmath>
#include <functional>
#include <limits>
#include <sstream>

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

void validateSizeT(const std::string& s)
{
    const auto notdigit = [](auto c) { return !std::isdigit(c); };
    auto itr = boost::find_if(s, notdigit);
    if (itr != s.cend())
    {
        throw CLI::ValidationError("arg should be a number");
    }

    static const size_t maxdigits = std::floor(std::log10(std::numeric_limits<size_t>::max())) + 1;
    if (s.size() > maxdigits)
    {
        throw CLI::ValidationError("arg is too long to convert");
    }
}

struct SizeTConverter
{
    SizeTConverter& operator=(const std::string& s)
    {
        validateSizeT(s);
        std::stringstream ss;
        ss << s;
        ss >> v;
        return *this;
    }
    operator size_t()
    {
        return v;
    }
    std::size_t v;
};

} // namespace

Parser::Parser(std::ostream& err) :
    m_err(err)
{
    cofigureAdd();
    configureConnect();
    configureRemove();
    configureShow();
    configureGeneral();
}

Commands Parser::parse(std::string line)
{
    reset();

    for (auto& c : line) { c = std::tolower(c); }
    auto args = split(line);
    static const std::string arg0{""};

    static std::vector<const char*> cargs;
    cargs.clear();
    cargs.push_back(arg0.c_str());
    for (const auto& s : args) cargs.push_back(s.c_str());
    try
    {
        m_parser.parse(
            static_cast<int>(cargs.size()),
            cargs.data());
    }
    catch (const std::runtime_error& e)
    {
        m_err << e.what() << std::endl;
        return {};
    }
    return m_commands;
}

void Parser::cofigureAdd()
{
    auto* add = m_parser.add_subcommand("add");
    add->require_option();

    const auto addNodeCommand = [this](bool yes)
    {
        if (yes)
        {
            m_commands.push_back(model::AddNodeAction{});
        }
    };
    add->add_flag("-n,--node", addNodeCommand);

}
void Parser::configureConnect()
{
    static model::ConnectAction action{};
    const auto addCommand = [this]()
    {
        m_commands.push_back(action);
    };
    const auto resetAction = [](auto)
    {
        action.input = 0;
        action.output = 0;
    };

    auto* connect = m_parser.add_subcommand("connect");
    connect->callback(addCommand);
    connect->require_option();
    connect->preparse_callback(resetAction);

    using namespace std::placeholders;
    auto* input = connect->add_option<std::size_t, SizeTConverter>(
        "-i,--input,input",
        action.input);
    input->expected(1);
    input->required();

    auto* output = connect->add_option<std::size_t, SizeTConverter>(
        "-o,--output,output",
        action.output);
    output->expected(1);
    output->required();
}

void Parser::configureRemove()
{
    static model::RemoveNodeAction removeNodeAction{};
    static model::DisconnectAction disconnectAction{};

    const auto callback = [this]()
    {
        if (removeNodeAction.id)
        {
            m_commands.push_back(removeNodeAction);
        }
        if (disconnectAction.connectionid)
        {
            m_commands.push_back(disconnectAction);
        }
    };
    const auto resetActions = [](auto)
    {
        removeNodeAction.id = 0;
        disconnectAction.connectionid = 0;
    };

    auto* remove = m_parser.add_subcommand("remove");
    remove->require_option();
    remove->callback(callback);
    remove->preparse_callback(resetActions);

    remove->add_option<std::size_t, SizeTConverter>(
        "-n,--node",
        removeNodeAction.id);

    remove->add_option<std::size_t, SizeTConverter>(
        "-c,--connection",
        disconnectAction.connectionid);
}

void Parser::configureShow()
{
    static ShowCommand command{};
    const auto callback = [this]()
    {
        m_commands.push_back(command);
    };
    const auto preparse = [](auto)
    {
        command.option = ShowCommand::Nodes;
    };
    auto* show = m_parser.add_subcommand("show");
    show->callback(callback);
    show->preparse_callback(preparse);

    show->add_flag_callback(
        "-n,--nodes",
        [](){command.option = ShowCommand::Nodes;});

    show->add_flag_callback(
        "-c,--connections",
        [](){command.option = ShowCommand::Connections;});

}

void Parser::configureGeneral()
{
    auto* quit = m_parser.add_subcommand("quit", "close the app");
    const auto addQuitCommand = [this]() { m_commands.push_back(QuitCommand{}); };
    quit->callback(addQuitCommand);
}

void Parser::reset()
{
    m_commands.clear();
}

}
