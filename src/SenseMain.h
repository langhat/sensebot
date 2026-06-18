#ifndef SENSEMAIN_H
#define SENSEMAIN_H

#include <vector>
#include <string>

class SenseMain {
    std::vector<std::string> args;
public:
    SenseMain(std::vector<std::string>& args);
    SenseMain() = default;
    void run();
};

#endif