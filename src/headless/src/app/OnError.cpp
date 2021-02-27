#include <headless/app/OnError.hpp>

#include <iostream>

namespace enkidu::headless {

void onError(model::Error e)
{
    std::cerr << "Error: ";
    switch(e)
    {
    case model::Error::NodeNotFound:
        std::cerr << "Node not found";
        break;
    case model::Error::PortNotFound:
        std::cerr << "Port not found";
        break;
    case model::Error::ConnectionNotFound:
        std::cerr << "Connection not found";
        break;
    default:
        std::cerr << "Unknown error";
        break;
    }
    std::cerr << std::endl;
}

}
