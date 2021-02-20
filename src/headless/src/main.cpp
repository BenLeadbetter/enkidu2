#include <headless/app/Application.hpp>

int main(int argc, char** argv)
{
    enkidu::headless::Application app(argc, argv);
    return app.run();
}
