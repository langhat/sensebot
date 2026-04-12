#include "network.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <ostream>

const char* MatchFailed::what() const noexcept {
    return "SyntaxError : Match failed";
}

network::network(int line, int cloumn) :
    neurons(line * cloumn),
    line(line),
    cloumn(cloumn) {
    for (int index = 0; index < cloumn; index++) {
        neurons[index] = new input{ 0.0 };
    }

    for (int index = cloumn; index < line * cloumn; index++) {
        neurons[index] = new norneuron{};
    }
    for (int l = 1; l < line; l++) {
        for (int i = 0; i < cloumn; i++) {
            norneuron* ptr = dynamic_cast<norneuron*>(neurons[l * cloumn + i]);
            if (!ptr) continue;
            for (int j = 0; j < cloumn; j++) {
                neuron* prev = neurons[(l - 1) * cloumn + j];
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
    if (inputs.size() != static_cast<size_t>(cloumn)) throw MatchFailed{};
    for (auto index = 0; index < cloumn; index++)
        neurons[index]->set_inside(inputs[index]);
    std::vector<real> result;
    for (auto index = (line - 1) * cloumn; index < line * cloumn; ++index)
        result.push_back(neurons[index]->count());
    return result;
}

void network::train(std::vector<real> inputs, std::vector<real> targets, real learning_rate) {
    // 检查输入和目标的大小是否匹配
    if (inputs.size() != static_cast<size_t>(cloumn)) {
        throw MatchFailed{};
    }
    if (targets.size() != static_cast<size_t>(cloumn)) {
        throw MatchFailed{};
    }
    
    // 前向传播
    auto outputs = count(inputs);
    
    // 计算输出层误差
    std::vector<real> errors(cloumn);
    for (int i = 0; i < cloumn; i++) {
        errors[i] = targets[i] - outputs[i];
    }
    
    // 反向传播误差并调整权重
    // 注意：这里简化了反向传播，实际实现可能需要更复杂的算法
    // 特别是对于多层网络，需要计算每一层的误差
    for (int l = line - 1; l > 0; l--) {
        for (int i = 0; i < cloumn; i++) {
            norneuron* ptr = dynamic_cast<norneuron*>(neurons[l * cloumn + i]);
            if (!ptr) continue;
            
            real error = (l == line - 1) ? errors[i] : 0.0; // 简化处理，只考虑输出层误差
            
            // 调整权重
            for (int j = 0; j < cloumn; j++) {
                neuron* prev = neurons[(l - 1) * cloumn + j];
                real input_val = prev->count();
                // 简化的权重更新规则
                real weight_update = learning_rate * error * input_val;
                // 获取当前权重并更新
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
    
    // 保存网络结构
    ofs << line << " " << cloumn << std::endl;
    
    // 保存权重
    for (int l = 1; l < line; l++) {
        for (int i = 0; i < cloumn; i++) {
            norneuron* ptr = dynamic_cast<norneuron*>(neurons[l * cloumn + i]);
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
    
    // 读取网络结构
    int loaded_line, loaded_cloumn;
    ifs >> loaded_line >> loaded_cloumn;
    
    // 检查网络结构是否匹配
    if (loaded_line != line || loaded_cloumn != cloumn) {
        return false;
    }
    
    // 读取权重
    for (int l = 1; l < line; l++) {
        for (int i = 0; i < cloumn; i++) {
            norneuron* ptr = dynamic_cast<norneuron*>(neurons[l * cloumn + i]);
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
