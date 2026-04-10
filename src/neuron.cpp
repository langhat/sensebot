#ifndef NEURON_CPP
#define NEURON_CPP

#include <vector>
#include <cmath>

using std::exp;
using real = long double;

class neuron {
public:
	virtual ~neuron() = default;
	virtual real count() = 0;
	virtual void set_inside(real) {}
};

class norneuron : public neuron {
protected:
	std::vector<neuron*> source;
	std::vector<real>    ws;
public:
	norneuron() = default;
	virtual real active(real in) {
		return (in > 0) ? in : 0;
	}
	virtual real count() {
		real result = 0.0;
		int index = 0;
		for (const auto& ptr : source) {
			if (ptr) {
				result += ptr->count() * ws[index];
			}
			index++;
		}
		return active(result);
	}
	void add_connection(neuron* src, real weight) {
		source.push_back(src);
		ws.push_back(weight);
	}
};

class rneuron : public norneuron {
	real store;
	real fweight;
public:
	rneuron() : store(0.0), fweight(0.0) {
		fweight = (std::rand() / (RAND_MAX + 1.0)) - 0.5;
	}
	real forget(real memory) {
		return 1.0 / (1.0 + exp(-memory));
	}
	real count() {
		real result = 0.0;
		int index = 0;
		for (const auto& ptr : source) {
			if (ptr) {
				result += ptr->count() * ws[index];
			}
			index++;
		}
		result += store * fweight;
		store = active(result);
		return store;
	}
	void set_fweight(real w) {
		fweight = w;
	}
};

class input : public neuron {
	real inside;
public:
	input(real inv = 0.0) : inside(inv) {}
	real count() {
		return inside;
	}
	void set_inside(real inv) {
		inside = inv;
	}
};

#endif