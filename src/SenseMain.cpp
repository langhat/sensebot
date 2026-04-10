#pragma once

#include <iostream>
#include <fstream>
#include "network.cpp"

#ifdef _WIN32

#include <windows.h>
#define sleep(TfS) Sleep(1000 * (TfS))

#else

#include <unistd.h>

#endif

using std::string;
using namespace std;

class SenseMain {
	std::vector<string> args;
	//network nw;
public:
	SenseMain(std::vector<string> &args):
		args(std::move(args)) {
		
	}
	SenseMain() {}

	void run() {
		using std::cout;
		for(auto index = 0; index < args.size(); index++) {
			if (args[index] == "test") {
				cout << "Training...\npls wait\n";
				sleep(1);
				cout << "End!";
				//TODO
			}
			else if(args[index] == "load") {
				string fina = args[++index];
				ifstream ifp(fina);
				string content, temp;
				while(getline(ifp, temp)) {
					content += temp;
				}
				cout << "(" << content << ")\n";
			}
			else if(args[index] == "init") {
				string fina = args[++index];
				ofstream ofp(fina);
			} else if(args[index] == "demo") {
				network nw(2, 3);
				std::vector<real> in(2);
				using std::cin;
				cout << ": " << std::flush;
				cin >> in[0] >> in[1];
				for(const auto &ec: nw.count(in)) {
					cout << ec << " ";
				}
				cout << std::endl;
			}
		}
	}
};