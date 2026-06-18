#include "neuron.h"
#include <cstdlib>

real norneuron::active(real in) {
    return (in > 0) ? in : 0;
}

real norneuron::count() {
    real result = 0.0;
    int index = 0;
    for (const auto& ptr : source) {
        if (ptr) {
            result += ptr->count() * ws[index];
        }
        index++;
    }
    return active(result);
}

void norneuron::add_connection(neuron* src, real weight) {
    source.push_back(src);
    ws.push_back(weight);
}

real norneuron::get_weight(size_t index) const {
    if (index < ws.size()) {
        return ws[index];
    }
    return 0.0;
}

void norneuron::set_weight(size_t index, real weight) {
    if (index < ws.size()) {
        ws[index] = weight;
    }
}

size_t norneuron::get_connection_count() const {
    return ws.size();
}

rneuron::rneuron() : store(0.0), fweight(0.0) {
    fweight = (std::rand() / (RAND_MAX + 1.0)) - 0.5;
}

real rneuron::forget(real memory) {
    return 1.0 / (1.0 + exp(-memory));
}

real rneuron::count() {
    real result = 0.0;
    int index = 0;
    for (const auto& ptr : source) {
        if (ptr) {
            result += ptr->count() * ws[index];
        }
        index++;
    }
    result += store * fweight;
    store = active(result);
    return store;
}

void rneuron::set_fweight(real w) {
    fweight = w;
}

input::input(real inv) : inside(inv) {}

real input::count() {
    return inside;
}

void input::set_inside(real inv) {
    inside = inv;
}