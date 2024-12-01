#define _USE_MATH_DEFINES
#include"Function.h"


Function::Function( std::string expression,QGraphicsScene* sceneS)
    : scene(sceneS),infixExpression(expression), variableManager(), parser(), evaluator()
{
    postfixExpression = parser.infixToPostfix(parser.correctInfixExpressionBySpacebars(infixExpression));
    variableManager.setParam(postfixExpression);
    variables = variableManager.getVariables();
}

void Function::setX(double x){
    variables= variableManager.getVariables();
    variables["e"]=variables["E"]=M_E;
    variables["pi"]=variables["Pi"]=M_PI;
    variables["x"]=x;
    variables["X"]=x;
    variables["y"]=1;
    variables["Y"]=1;
}

void Function::setTheta(double theta){
    variables= variableManager.getVariables();
    variables["e"]=variables["E"]=M_E;
    variables["pi"]=variables["Pi"]=M_PI;
    variables["theta"]=theta;
    variables["r"]=1;
    variables["R"]=1;
}

double Function::evaluate() {
    return evaluator.evaluatePostfix(postfixExpression, variables);
}


