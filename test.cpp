#include <iostream>
#include <vector>
#include "src/network.h"

using namespace std;

int main() {
    // 测试网络创建和前向传播
    cout << "Testing network creation and forward propagation..." << endl;
    network nw(3, 2);
    
    // 测试输入数据
    vector<real> inputs = {0.5, 0.5};
    auto outputs = nw.count(inputs);
    cout << "Input: 0.5, 0.5 -> Output: " << outputs[0] << ", " << outputs[1] << endl;
    
    // 测试训练功能
    cout << "\nTesting training functionality..." << endl;
    // 训练数据：XOR逻辑门
    vector<vector<real>> train_inputs = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
    };
    
    vector<vector<real>> train_targets = {
        {0, 1}, // 0 XOR 0 = 0
        {1, 0}, // 0 XOR 1 = 1
        {1, 0}, // 1 XOR 0 = 1
        {0, 1}  // 1 XOR 1 = 0
    };
    
    // 训练参数
    int epochs = 1000;
    real learning_rate = 0.1;
    
    // 开始训练
    for (int epoch = 0; epoch < epochs; epoch++) {
        for (size_t i = 0; i < train_inputs.size(); i++) {
            nw.train(train_inputs[i], train_targets[i], learning_rate);
        }
        
        // 每100个epoch输出一次训练状态
        if (epoch % 100 == 0) {
            cout << "Epoch " << epoch << " completed" << endl;
        }
    }
    
    // 测试训练结果
    cout << "\nTraining completed! Testing results:" << endl;
    for (size_t i = 0; i < train_inputs.size(); i++) {
        auto test_outputs = nw.count(train_inputs[i]);
        cout << "Input: " << train_inputs[i][0] << ", " << train_inputs[i][1] << " -> Output: " << test_outputs[0] << ", " << test_outputs[1] << " (Expected: " << train_targets[i][0] << ", " << train_targets[i][1] << ")" << endl;
    }
    
    // 测试模型保存和加载
    cout << "\nTesting model save and load..." << endl;
    string filename = "test_model.txt";
    if (nw.save(filename)) {
        cout << "Model saved successfully!" << endl;
        
        // 创建新网络并加载模型
        network nw_loaded(3, 2);
        if (nw_loaded.load(filename)) {
            cout << "Model loaded successfully!" << endl;
            
            // 测试加载的模型
            cout << "\nTesting loaded model:" << endl;
            for (size_t i = 0; i < train_inputs.size(); i++) {
                auto loaded_outputs = nw_loaded.count(train_inputs[i]);
                cout << "Input: " << train_inputs[i][0] << ", " << train_inputs[i][1] << " -> Output: " << loaded_outputs[0] << ", " << loaded_outputs[1] << " (Expected: " << train_targets[i][0] << ", " << train_targets[i][1] << ")" << endl;
            }
        } else {
            cout << "Failed to load model!" << endl;
        }
    } else {
        cout << "Failed to save model!" << endl;
    }
    
    return 0;
}
