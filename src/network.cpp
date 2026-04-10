#pragma once

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "neuron.cpp"

class MatchFailed : public std::exception {
public:
	const char* what()const noexcept {
		return "SyntaxError : Match failed";
	}
};

class network {
	std::vector<neuron*> neurons;
	int line, cloumn;
public:
	network(int line, int cloumn) :
		neurons(line* cloumn),
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
	~network() {
		for (auto& each : neurons) {
			delete each;
		}
	}
	std::vector<real> count(std::vector<real> inputs) {
		if (inputs.size() != static_cast<size_t>(cloumn)) throw MatchFailed{};
		for (auto index = 0; index < cloumn; index++)
			neurons[index]->set_inside(inputs[index]);
		std::vector<real> result;
		for (auto index = (line - 1) * cloumn; index < line * cloumn; ++index)
			result.push_back(neurons[index]->count());
		return result;
	}
};