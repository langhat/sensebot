#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include "network.cpp"

#ifdef _WIN32

#include <windows.h>
#define sleep(TfS) Sleep(1000 * (TfS))

#else

#include <unistd.h>

#endif

using std::string;

class SenseMain {
	std::vector<string> args;
public:
	SenseMain(std::vector<string>& args) :
		args(std::move(args)) {

	}
	SenseMain() = default;

	void run() {
		using std::cout;
		using std::cerr;
		for (size_t index = 0; index < args.size(); index++) {
			if (args[index] == "test") {
				cout << "Training...\npls wait\n";
				sleep(1);
				cout << "End!";
			}
			else if (args[index] == "load") {
				if (index + 1 >= args.size()) {
					std::cerr << "Error: 'load' requires a filename argument.\n";
					continue;
				}
				string fina = args[++index];
				std::ifstream ifp(fina);
				if (!ifp) {
					cerr << "Error: cannot open file '" << fina << "'\n";
					continue;
				}
				string content, temp;
				while (std::getline(ifp, temp)) {
					content += temp;
				}
				cout << "(" << content << ")\n";
			}
			else if (args[index] == "init") {
				if (index + 1 >= args.size()) {
					cerr << "Error: 'init' requires a filename argument.\n";
					continue;
				}
				string fina = args[++index];
				std::ofstream ofp(fina);
			}
			else if (args[index] == "demo") {
				network nw(2, 2);
				std::vector<real> in(2);
				cout << ": " << std::flush;
				using std::cin;
				cin >> in[0] >> in[1];
				for (const auto& ec : nw.count(in)) {
					cout << ec << " ";
				}
				cout << std::endl;
			}
			else {
				cerr << "Unknown command: " << args[index] << "\n";
			}
		}
	}
};