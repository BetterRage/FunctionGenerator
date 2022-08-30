#include "CombinedFunction.hpp"

#include <array>
#include <numeric>

void CombinedFunction::addFunction(FunctionBase* function) {
    functions.push_back(function);
}

double CombinedFunction::calculate(int64_t x) {
    double result = 0;
    for (size_t i = 0; i < functions.size(); i++)
        result += functions[i]->calculate(x);
    return result;
}
