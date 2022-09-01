#include <cmath>
#include <inttypes.h>
#include <memory>
#include <vector>

class FunctionBase
{
  public:
	virtual double calculate(int64_t t) = 0;
	virtual ~FunctionBase() {}
};

typedef struct {
	double amplitude = 1, frequency = 1, offset = 1, phaseshift = 0;
} SineParams;

class Sine : public FunctionBase
{
  public:
	// frequency: Hz
	// phase: rad
	Sine(SineParams params)
	{
		_amp = params.amplitude;
		_dc = params.offset;
		// if time is 1.000.000 (1 second) then we should have a full period
		_freq = params.frequency * 2.0 * M_PI / 1000000.0;
		// time is in microseconds so a phase shift of 2 * PI * 1000000 is a
		// full period
		_phase = params.phaseshift;
	}

	// x = time in microseconds
	inline double calculate(int64_t t) override { return _amp * sin(_freq * t + _phase) + _dc; }

  private:
	double _amp, _freq, _dc, _phase;
};

typedef struct {
	double amplitude = 1, frequency = 1, offset = 1, duty = 50;
} SquareParams;

class Square : public FunctionBase
{
  public:
	// duty: %
	// frequency: Hz
	Square(SquareParams params)
	{
		_lhigh = params.offset + params.amplitude;
		_llow = params.offset - params.amplitude;

		_periodTime = 1000000.0 / params.frequency;
		_highTime = _periodTime * params.duty / 100.0;
	}

	// x = time in microseconds
	double calculate(int64_t t) override
	{
		int period = t / _periodTime;
		double timeinperiod = t - period * _periodTime;
		if(timeinperiod <= _highTime)
			return _lhigh;
		return _llow;
	}

  private:
	double _periodTime, _highTime, _lhigh, _llow;
};

typedef struct {
	double amplitude = 1, frequency = 1, offset = 1, risetime = 50;
} TriangularParams;

class Triangular : public FunctionBase
{
  public:
	// risetime: %
	// frequency: Hz
	Triangular(TriangularParams params)
	{
		_periodTime = 1000000.0 / params.frequency;
		_amplitude = params.amplitude;
		_llow = params.offset - params.amplitude / 2;
		_riseTime = _periodTime * params.risetime / 100.0;
		_fallTime = _periodTime - _riseTime;
	}

	// x = time in microseconds
	double calculate(int64_t t) override
	{
		int period = t / _periodTime;
		double timeinperiod = t - period * _periodTime;
		if(timeinperiod <= _riseTime)
			return _llow + (timeinperiod / _riseTime) * _amplitude;
		double timeFalling = _fallTime - (timeinperiod - _riseTime);
		return _llow + timeFalling / _fallTime * _amplitude;
	}

  private:
	double _periodTime, _riseTime, _fallTime, _amplitude, _llow;
};

class CombinedFunction
{
  public:
	double calculate(int64_t x)
	{
		double result = 0;
		for(size_t i = 0; i < functions.size(); i++)
			result += functions[i]->calculate(x);
		return result;
	}

	template <typename function, typename param> void addFunction(param params)
	{
		std::unique_ptr<FunctionBase> func = std::make_unique<function>(params);
		functions.push_back(std::move(func));
	}

  private:
	std::vector<std::unique_ptr<FunctionBase>> functions;
};
