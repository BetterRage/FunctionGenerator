#include "FunctionGen.hpp"
#include <chrono>
#include <stdio.h>

int main(void)
{
    TriangularParams triparams = {
        .amplitude=25,
        .frequency=1,
        .offset=0,
        .risetime=50
    };
    SquareParams squareparams{
        .amplitude=20,
        .frequency=1,
        .offset=10,
        .duty=50
    };
    CombinedFunction cf;
    cf.addFunction<Triangular,TriangularParams>(triparams);
    cf.addFunction<Square,SquareParams>(squareparams);
    printf("%f\n",cf.calculate(500000)); //give time in microseconds

    //output is 55.0
    //at 0.5 seconds the sawtooth reaches peak (25) + square high (20) + offset (10) = 55.0
}