#pragma once

#include"Header.h"
#include"MathHeader.h"
#include"Parser.h"
#include"Cartesian.h"

class Function {
public:
    Function(const std::string& expression);
    void setX(double x);
    double evaluate();

private:
    std::string infixExpression;
    std::vector<std::string> postfixExpression;
    VariableManager variableManager;
    Parser parser;
    Evaluator evaluator;
    std::map<std::string, double> variables;
    Math& functionsMath = Math::getInstance();
};
