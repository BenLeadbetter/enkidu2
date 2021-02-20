#include <headless/app/Application.hpp>

namespace enkidu::headless {

Application::Application(int, char**)
{

}

int Application::run()
{
    while(true)
    {
        m_input.get();
    }
    return 0;
}

}
