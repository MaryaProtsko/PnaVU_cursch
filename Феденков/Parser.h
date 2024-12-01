#pragma once
#include"Header.h"
#include"Accesory.h"
#include"MathHeader.h"
#include<QWidget>

class Parser {
public:
    std::vector<std::string> infixToPostfix(const std::string& expression);
    std::string correctInfixExpressionBySpacebars(const std::string& infixExpression);
private:
    void handleOperator(const std::string& temp, std::stack<std::string>& stack, std::vector<std::string>& result);
    Math& functionsMath = Math::getInstance();
    AccesoryChecker entityQualities;
};


class VariableManager {
public:
    VariableManager() = default;
    void setParam(const std::vector<std::string>& postfixExpression);
    const std::map<std::string, double>& getVariables() const;
    double getVariableValue(const std::string& variableName, QWidget* parent=nullptr);

private:
    std::map<std::string, double> variables;
    AccesoryChecker entityQualities;
};



class Evaluator {
public:
    double evaluatePostfix(const std::vector<std::string>& postfix, std::map<std::string, double>& variables);
private:
    Math& functionsMath = Math::getInstance();
    AccesoryChecker entityQualities;
};


