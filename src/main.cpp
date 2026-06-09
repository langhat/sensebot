#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "SenseMain.h"

int main(int argc, char** argv) {
	using std::vector;
	using std::string;
	vector<string> args;
	for (auto index = 1; index < argc; index++) {
		args.push_back(argv[index]);
	}
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	SenseMain sm(args);
	sm.run();
	std::cout << "Press Enter to exit...";
	std::cin.get();
}