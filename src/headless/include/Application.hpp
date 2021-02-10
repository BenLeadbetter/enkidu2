#pragma once

#include <QCoreApplication>

namespace enkidu::headless {

class Application : public QCoreApplication
{
public:
    Application(int&, char**);
};

} // namespace enkidu::headless
