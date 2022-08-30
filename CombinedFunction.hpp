#include "SimpleFunction.hpp"
#include <vector>

class CombinedFunction
{
public:
    double calculate(int64_t x);
    void addFunction(FunctionBase* function);
private:
    std::vector<FunctionBase*> functions;
};