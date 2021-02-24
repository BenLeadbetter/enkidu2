#include <headless/app/Application.hpp>

namespace enkidu::headless {

Application::Application(int, char**) :
    m_dispatchCommand(m_store, m_return)
{}

int Application::run()
{
    try
    {
        while(!m_return)
        {
            auto commands = m_input.get();
            for (const auto& c : commands)
            {
                std::visit(m_dispatchCommand, c);
            }
        }
    }
    catch(...)
    {
        if (!m_return || *m_return == 0)
        {
            m_return = 1;
        }
    }
    return *m_return;
}

}
