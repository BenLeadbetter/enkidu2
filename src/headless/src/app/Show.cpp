#include <headless/app/Show.hpp>

#include <iomanip>
#include <ios>
#include <iostream>

namespace enkidu::headless {

void show(model::Node node)
{
    std::cout <<
        "Node" << std::right << std::setw(12) <<
        "id: " << node.id << std::endl;

    std::cout << std::setw(12) << "";
    std::cout << std::left << std::setw(12) << "Input ports:" << std::endl;
    for (const auto& p : node.inputs)
    {
        std::cout << std::right << std::setw(24) << p.id << std::endl;
    }

    std::cout << std::setw(12) << "";
    std::cout << std::left << std::setw(12) << "Output ports:" << std::endl;
    for (const auto& p : node.outputs)
    {
        std::cout << std::right << std::setw(24) << p.id << std::endl;
    }
}

void show(model::Connection c)
{
    std::cout << std::left << std::setw(12) << "Connection";
    std::cout << std::setw(10) << "id: "  << std::setw(16) << c.id;
    std::cout << std::setw(10) << "input: " << std::setw(16)  << c.input.portid;
    std::cout << std::setw(10) << "output: " << std::setw(16)  << c.output.portid;
    std::cout << std::endl;
}

}
