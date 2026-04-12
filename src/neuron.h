#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <cmath>

using std::exp;
typedef long double real;

class neuron {
public:
    virtual ~neuron() = default;
    virtual real count() = 0;
    virtual void set_inside(real) {};
};

class norneuron : public neuron {
protected:
    std::vector<neuron*> source;
    std::vector<real>    ws;
public:
    norneuron() = default;
    virtual real active(real in);
    virtual real count();
    void add_connection(neuron* src, real weight);
    real get_weight(size_t index) const;
    void set_weight(size_t index, real weight);
    size_t get_connection_count() const;
};

class rneuron : public norneuron {
    real store;
    real fweight;
public:
    rneuron();
    real forget(real memory);
    real count() override;
    void set_fweight(real w);
};

class input : public neuron {
    real inside;
public:
    input(real inv = 0.0);
    real count() override;
    void set_inside(real inv) override;
};

#endif