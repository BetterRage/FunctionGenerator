#include <inttypes.h>
#include <cmath>
#include <vector>
#include "SimpleFunction.hpp"

class FunctionBase {
   public:
	virtual double calculate(int64_t t) = 0;
};

class Sine : public FunctionBase {
   public:
	// a = amplitude
	// b = frequency in hz
	// c = y-offset
	// d = phase shift in radians
	Sine(double amplitude = 1, double frequency = 1, double offset = 0,
		 double phase = 0) {
		_A = amplitude;
		_C = offset;
		// if time is 1.000.000 (1 second) then we should have a full period
		_B = frequency * 2.0 * M_PI / 1000000.0;
		// time is in microseconds so a phase shift of 2 * PI * 1000000 is a
		// full period
		_D = phase;
	}

	// x = time in microseconds
	inline double calculate(int64_t t) override {
		return _A * sin(_B * t + _D) + _C;
	}

   private:
	double _A, _B, _C, _D;
};

class CombinedFunction {
   public:
	double calculate(int64_t x) {
		double result = 0;
		for (size_t i = 0; i < functions.size(); i++)
			result += functions[i]->calculate(x);
		return result;
	}

	template <typename Tp, typename... Args>
	void add(Args... args) {
		FunctionBase* func = new Tp(args...);
		functions.push_back(func);
	}

   private:
	std::vector<FunctionBase*> functions;
};

