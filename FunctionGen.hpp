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

class Sine : public FunctionBase
{
  public:
	// frequency: Hz
	// phase: rad
	Sine(double amplitude, double frequency, double offset = 0, double phase = 0)
	{
		_amp = amplitude;
		_dc = offset;
		// if time is 1.000.000 (1 second) then we should have a full period
		_freq = frequency * 2.0 * M_PI / 1000000.0;
		// time is in microseconds so a phase shift of 2 * PI * 1000000 is a
		// full period
		_phase = phase;
	}

	// x = time in microseconds
	inline double calculate(int64_t t) override { return _amp * sin(_freq * t + _phase) + _dc; }

  private:
	double _amp, _freq, _dc, _phase;
};

class Square : public FunctionBase
{
  public:
	// duty: %
	Square(double amplitude, double frequency, double offset = 0, double duty = 50)
	{
		_lhigh = offset + amplitude;
		_llow = offset - amplitude;

		_periodTime = 1000000.0 / frequency;
		_highTime = _periodTime * duty / 100.0;
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
	double _periodTime, _highTime, _phase, _duty, _lhigh, _llow;
};

class Triangular : public FunctionBase
{
  public:
	// risetime: %
	Triangular(double amplitude, double frequency, double offset, double risetime)
	{
		_periodTime = 1000000.0 / frequency;
		_amplitude = amplitude;
		_llow = offset - amplitude / 2;
		_riseTime = _periodTime * risetime / 100.0;
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

	template <typename Tp, typename... Args> void add(Args... args)
	{
		std::unique_ptr<FunctionBase> func = std::make_unique<Tp>(args...);
		functions.push_back(std::move(func));
	}

  private:
	std::vector<std::unique_ptr<FunctionBase>> functions;
};
