#include <Application.hpp>


namespace enkidu::headless {

Application::Application(int& argc, char** argv) :
    QCoreApplication(argc, argv)
{
    setApplicationName("enkidu-headless");
}

}
