#include <iostream>
#include <vector>
#include "SenseMain.cpp"

int main(int argc, char **argv) {
	using std::vector;
	using std::string;
	vector<string> args;
	for(auto index = 1; index < argc; index++) {
		args.push_back(argv[index]);
	}
	SenseMain sm(args);
	sm.run();
}