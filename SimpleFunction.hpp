#include <inttypes.h>


const double PI = 3.1415926535897932;

class FunctionBase
{
public:
    virtual double calculate(int64_t t) = 0;
};

class Sine : public FunctionBase
{
public:
    //a = amplitude 
    //b = frequency in hz 
    //c = y-offset 
    //d = phase shift in radians
    Sine(double a = 1,double b = 1,double c = 0,double d = 0);

    //x = time in microseconds
    double calculate(int64_t t) override;
private:
    double _A,_B,_C,_D;
};