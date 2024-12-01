#pragma once
#include"Header.h"

class AccesoryChecker {
public:
    AccesoryChecker() : operatorSet(operators.operators.begin(), operators.operators.end()) {}
    ~AccesoryChecker() {}

    bool isNumber(char ch) const;
    bool isLetter(char ch) const;
    bool isOperator(const std::string& potentialOperator) const;
    bool isBinaryOperator(const std::string& potentialBinaryOperator) const;

private:
    Operators operators;
    std::unordered_set<std::string> operatorSet;
};
