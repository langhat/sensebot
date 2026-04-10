#pragma once

#include <iostream>
#include "neuron.cpp"

class MatchFailed: public std::exception {
public:
	const char *what()const noexcept{
		return "SyntaxError : Match failed";
	}
};

class network {
	std::vector<neuron *> neurons;
	int line, cloumn;
public:
	network (int line, int cloumn):
		neurons(line * cloumn),
		line(line),
		cloumn(cloumn) {
		for (int index = 0; index < cloumn; index++) {
			neurons[index] = new input{0};
		}
		for (int index = cloumn; index < line * cloumn; index++) {
			neurons[index] = new norneuron{};
		}
	}
	~network () {
		for (auto &each: neurons) {
			delete each;
		}
	}
	std::vector<real> count(std::vector<real> inputs) {
		if (inputs.size() != cloumn) throw MatchFailed{};
		for (auto index = 0; index < cloumn; index++)
			neurons[index]->set_inside(inputs[index]);
		std::vector<real> result;
		for (auto index = (line - 1) * cloumn; index <= line *cloumn; index++)
			result.push_back(neurons[index]->count());
		return result;
	}
	
};