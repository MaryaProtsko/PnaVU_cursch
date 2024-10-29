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


typedef struct Operands {
    std::vector< std::string> operands = { "=","+","-","*","/","^","sin","arcsin","asin",
                                           "cos","arccos","acos","tg","tan","arctg","arctan",
                                           "atg","atan","ctg","cot","arcctg","arccot","actg",
                                           "acot","sqrt","root","abs","log","lg","ln","#" };
}Operand;
