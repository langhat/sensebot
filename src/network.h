#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <string>
#include "neuron.h"

class MatchFailed : public std::exception {
public:
    const char* what() const noexcept override;
};

class network {
    std::vector<neuron*> neurons;
    int line, cloumn;
public:
    network(int line, int cloumn);
    ~network();
    std::vector<real> count(std::vector<real> inputs);
    void train(std::vector<real> inputs, std::vector<real> targets, real learning_rate);
    bool save(const std::string& filename) const;
    bool load(const std::string& filename);
};

#endif