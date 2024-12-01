#pragma once

#include"Header.h"
#include"MathHeader.h"
#include"Parser.h"
#include"DrawCartesian.h"
#include<QGraphicsScene>

class Function {
public:
    Function( std::string expression,QGraphicsScene* scene);
    void setX(double x);
    void setTheta(double theta);
    double evaluate();

private:
    QGraphicsScene* scene;
    std::string infixExpression;
    std::vector<std::string> postfixExpression;
    VariableManager variableManager;
    Parser parser;
    Evaluator evaluator;
    std::map<std::string, double> variables;
    Math& functionsMath = Math::getInstance();
};
