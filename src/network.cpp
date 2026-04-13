#include "network.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <ostream>

const char* MatchFailed::what() const noexcept {
    return "SyntaxError : Match failed";
}

network::network(int line, int column) :
    neurons(line * column),
    line(line),
    column(column) {
    for (int index = 0; index < column; index++) {
        neurons[index] = new input{ 0.0 };
    }

    for (int index = column; index < line * column; index++) {
        neurons[index] = new norneuron{};
    }
    for (int l = 1; l < line; l++) {
        for (int i = 0; i < column; i++) {
            norneuron* ptr = dynamic_cast<norneuron*>(neurons[l * column + i]);
            if (!ptr) continue;
            for (int j = 0; j < column; j++) {
                neuron* prev = neurons[(l - 1) * column + j];
                real weight = (std::rand() / (RAND_MAX + 1.0)) - 0.5;
                ptr->add_connection(prev, weight);
            }
        }
    }
}

network::~network() {
    for (auto& each : neurons) {
        delete each;
    }
}

std::vector<real> network::count(std::vector<real> inputs) {
    if (inputs.size() != static_cast<size_t>(column)) throw MatchFailed{};
    for (auto index = 0; index < column; index++)
        neurons[index]->set_inside(inputs[index]);
    std::vector<real> result;
    for (auto index = (line - 1) * column; index < line * column; ++index)
        result.push_back(neurons[index]->count());
    return result;
}

void network::train(std::vector<real> inputs, std::vector<real> targets, real learning_rate) {
    if (inputs.size() != static_cast<size_t>(column)) {
        throw MatchFailed{};
    }
    if (targets.size() != static_cast<size_t>(column)) {
        throw MatchFailed{};
    }
    
    auto outputs = count(inputs);
    
    std::vector<real> errors(column);
    for (int i = 0; i < column; i++) {
        errors[i] = targets[i] - outputs[i];
    }
    
    for (int l = line - 1; l > 0; l--) {
        for (int i = 0; i < column; i++) {
            norneuron* ptr = dynamic_cast<norneuron*>(neurons[l * column + i]);
            if (!ptr) continue;
            
            real error = (l == line - 1) ? errors[i] : 0.0;
            
            for (int j = 0; j < column; j++) {
                neuron* prev = neurons[(l - 1) * column + j];
                real input_val = prev->count();
                real weight_update = learning_rate * error * input_val;
                real current_weight = ptr->get_weight(j);
                ptr->set_weight(j, current_weight + weight_update);
            }
        }
    }
}

bool network::save(const std::string& filename) const {
    std::ofstream ofs(filename);
    if (!ofs) {
        return false;
    }
    
    ofs << line << " " << column << std::endl;
    
    for (int l = 1; l < line; l++) {
        for (int i = 0; i < column; i++) {
            norneuron* ptr = dynamic_cast<norneuron*>(neurons[l * column + i]);
            if (!ptr) continue;
            
            for (size_t j = 0; j < ptr->get_connection_count(); j++) {
                ofs << ptr->get_weight(j) << " ";
            }
            ofs << std::endl;
        }
    }
    
    return true;
}

bool network::load(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) {
        return false;
    }
    
    int loaded_line, loaded_column;
    ifs >> loaded_line >> loaded_column;
    
    if (loaded_line != line || loaded_column != column) {
        return false;
    }
    
    for (int l = 1; l < line; l++) {
        for (int i = 0; i < column; i++) {
            norneuron* ptr = dynamic_cast<norneuron*>(neurons[l * column + i]);
            if (!ptr) continue;
            for (size_t j = 0; j < ptr->get_connection_count(); j++) {
                real weight;
                ifs >> weight;
                ptr->set_weight(j, weight);
            }
        }
    }
    return true;
}