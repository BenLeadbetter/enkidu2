#include <Application.hpp>

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    enkidu::headless::Application app(argc, argv);

    while(true)
    {
        std::string line;
        std::cout << "enkidu >> ";
        std::cin >> line;

        if (line == "exit")
        {
            break;
        }
        else
        {
            std::cout << line << std::endl;
        }
        QCoreApplication::processEvents();
    }
    return 0;
}
