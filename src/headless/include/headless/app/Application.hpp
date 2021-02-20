#pragma once

#include <headless/app/DispatchCommand.hpp>
#include <headless/app/Input.hpp>

#include <headless/model/Store.hpp>

namespace enkidu::headless {

class Application
{
public:
    Application(int, char**);
    int run();

private:
    Input m_input;
    Store m_store;
    DispatchCommand m_dispatchCommand;
};

}
