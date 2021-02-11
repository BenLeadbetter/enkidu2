#pragma once

#include <QCoreApplication>

namespace enkidu {

class HeadlessApplication : public QCoreApplication
{
public:
    HeadlessApplication(int& argc, char** argv);
};

}
