#pragma once
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <cctype>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <stack>
#include <map>

class Operators {
public:
    std::vector< std::string> operators = { "=","+","-","*","/","^","sin","arcsin",
                                           "cos","arccos","tan","arctan",
                                           "cot","arccot",
                                           "sqrt","root","abs","log","lg","ln","#" };
};
