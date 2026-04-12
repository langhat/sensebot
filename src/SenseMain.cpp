#include "SenseMain.h"
#include <iostream>
#include <fstream>
#include <utility>
#include "network.h"

#ifdef _WIN32

#include <windows.h>
#define sleep(TfS) Sleep(1000 * (TfS))

#else

#include <unistd.h>

#endif

using std::string;

SenseMain::SenseMain(std::vector<string>& args) :
    args(std::move(args)) {

}

void SenseMain::run() {
    using std::cout;
    using std::cerr;
    try {
        for (size_t index = 0; index < args.size(); index++) {
            if (args[index] == "test") {
                cout << "Training...\npls wait\n";
                // 创建一个2层网络，输入层2个神经元，隐藏层2个神经元，输出层2个神经元
                network nw(3, 2);
                
                // 训练数据：XOR逻辑门
                std::vector<std::vector<real>> inputs = {
                    {0, 0},
                    {0, 1},
                    {1, 0},
                    {1, 1}
                };
                
                std::vector<std::vector<real>> targets = {
                    {0, 1}, // 0 XOR 0 = 0
                    {1, 0}, // 0 XOR 1 = 1
                    {1, 0}, // 1 XOR 0 = 1
                    {0, 1}  // 1 XOR 1 = 0
                };
                
                // 训练参数
                int epochs = 10000;
                real learning_rate = 0.1;
                
                // 开始训练
                for (int epoch = 0; epoch < epochs; epoch++) {
                    for (size_t i = 0; i < inputs.size(); i++) {
                        nw.train(inputs[i], targets[i], learning_rate);
                    }
                    
                    // 每1000个epoch输出一次训练状态
                    if (epoch % 1000 == 0) {
                        cout << "Epoch " << epoch << " completed\n";
                    }
                }
                
                // 测试训练结果
                cout << "Training completed! Testing results:\n";
                for (size_t i = 0; i < inputs.size(); i++) {
                    auto outputs = nw.count(inputs[i]);
                    cout << "Input: " << inputs[i][0] << ", " << inputs[i][1] << " -> Output: " << outputs[0] << ", " << outputs[1] << " (Expected: " << targets[i][0] << ", " << targets[i][1] << ")\n";
                }
                cout << "End!";
            }
            else if (args[index] == "load") {
                if (index + 1 >= args.size()) {
                    std::cerr << "Error: 'load' requires a filename argument.\n";
                    continue;
                }
                string fina = args[++index];
                network nw(3, 2); // 创建与保存时相同结构的网络
                if (nw.load(fina)) {
                    cout << "Network loaded successfully!\n";
                    // 测试加载的网络
                    std::vector<real> test_input = {0, 1};
                    auto outputs = nw.count(test_input);
                    cout << "Test input: 0, 1 -> Output: " << outputs[0] << ", " << outputs[1] << "\n";
                } else {
                    cerr << "Error: failed to load network from file '" << fina << "'\n";
                }
            }
            else if (args[index] == "init") {
                if (index + 1 >= args.size()) {
                    cerr << "Error: 'init' requires a filename argument.\n";
                    continue;
                }
                string fina = args[++index];
                network nw(3, 2); // 创建一个新的网络
                if (nw.save(fina)) {
                    cout << "Network initialized and saved successfully!\n";
                } else {
                    cerr << "Error: failed to initialize network\n";
                }
            }
            else if (args[index] == "demo") {
                network nw(2, 2);
                std::vector<real> in(2);
                cout << ": " << std::flush;
                using std::cin;
                cin >> in[0] >> in[1];
                try {
                    auto outputs = nw.count(in);
                    for (const auto& ec : outputs) {
                        cout << ec << " ";
                    }
                    cout << std::endl;
                } catch (const MatchFailed& e) {
                    cerr << "Error: " << e.what() << "\n";
                }
            }
            else {
                cerr << "Unknown command: " << args[index] << "\n";
            }
        }
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }
}
