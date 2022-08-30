#include "SimpleFunction.hpp"

#include <cmath>

Sine::Sine(double a, double b, double c, double d) 
{
    _A = a;
    
    //if time is 1.000.000 (1 second) then we should have a full period
    _B = b * 2.0 * PI / 1000000.0;
    _C = c;

    //time is in microseconds so a phase shift of 2 * PI * 1000000 is a full period
    _D = d;

}

double Sine::calculate(int64_t t) 
{
    return _A * sin(_B * t + _D) + _C; 
}
