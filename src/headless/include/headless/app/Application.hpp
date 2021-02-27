#pragma once

#include <headless/app/DispatchCommand.hpp>
#include <headless/app/Input.hpp>

#include <headless/model/Store.hpp>

#include <optional>

namespace enkidu::headless {

class Application
{
public:
    Application(int, char**);
    int run();

private:
    void initSubscribers();

    std::optional<int> m_return;
    Input m_input;
    Store m_store;
    DispatchCommand m_dispatchCommand;
};

}
