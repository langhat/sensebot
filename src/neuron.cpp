#ifndef NEURON_CPP
#define NEURON_CPP

#include <vector>
#include <cmath>

using std::exp;
using real = long double;

class neuron {
public:
	virtual real count() = 0;
	virtual void set_inside(real) {}
};

class norneuron: public neuron {
protected:
	std::vector<neuron*> source;
	std::vector<real>    ws;
public:
	norneuron() {
		
	}
	virtual real active(real in) {
		return (in > 0) ? in : 0;
	}
	virtual real count() {
		real result = 0.0;
		int index = 0;
		for (const auto &ptr: source) {
			if(ptr) {
				result += ptr->count() * ws[index];
			}
			index++;
		}
		return active(result);
	}

};

class rneuron: public norneuron {
	real store;
public:
	real forget(real memory) {
	    return 1.0 / (1.0 + exp(-memory));
	}
	real count() {
		real result = 0.0;
		int index=0;
		for (const auto &ptr: source) {
			if(ptr) {
				result += ptr->count() * ws[index];
			}
			index++;
		}
		result = store * ws[index];
		store = active(result);

		return store = active(result);
	}
};

class input: public neuron {
	real inside;
public:
	input (real inv = 0.0): inside(inv) {}
	real count() {
		return inside;
	}
	void set_inside(real inv) {
		inside = inv;
	}
};

#endif