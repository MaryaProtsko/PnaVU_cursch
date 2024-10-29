#define _USE_MATH_DEFINES
#include"Function.h"


Function::Function(const std::string& expression)
    : infixExpression(expression), variableManager(), parser(), evaluator(functionsMath)
{
    postfixExpression = parser.infixToPostfix(parser.correctInfixExpressionBySpacebars(infixExpression));
    //variableManager.setParam(postfixExpression);
    //variables = variableManager.getVariables();
}

void Function::setX(double x){
    variables= variableManager.getVariables();
    variables["x"]=x;
    variables["X"]=x;
}


double Function::evaluate() {
    return evaluator.evaluatePostfix(postfixExpression, variables);
}


