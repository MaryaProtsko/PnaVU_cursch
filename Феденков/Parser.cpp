#include"Parser.h"

std::vector<std::string> Parser::infixToPostfix(const std::string& expression) {
    std::vector<std::string> result;
    std::stack<std::string> stack;
    std::string temp;
    bool lastWasOperator = true;

    for (char ch : expression) {
        if (ch != ' ' && ch != '(' && ch != ')') {
            temp.push_back(ch);
        }

        if (ch == ' ') {
            if (!temp.empty()) {
                if (!entityQualities.isOperator(temp)) {
                    result.push_back(temp);
                    lastWasOperator = false;
                }
                else {
                    handleOperator(temp, stack, result);
                    lastWasOperator = true;
                }
                temp.clear();
            }
        }

        if (ch == '(') {
            stack.push("(");
            lastWasOperator = true;
        }

        if (temp == "-" && lastWasOperator) {
            temp = "#"; // ”нарный минус
        }

        if (ch == ')') {
            while (!stack.empty() && stack.top() != "(") {
                result.push_back(stack.top());
                stack.pop();
            }
            stack.pop();
            lastWasOperator = false;
        }
    }

    while (!stack.empty()) {
        result.push_back(stack.top());
        stack.pop();
    }

    return result;
}

void Parser::handleOperator(const std::string& temp, std::stack<std::string>& stack, std::vector<std::string>& result) {
    while (!stack.empty() && (functionsMath.getPrecedence(temp) < functionsMath.getPrecedence(stack.top()) ||
        (functionsMath.getPrecedence(temp) == functionsMath.getPrecedence(stack.top()) && functionsMath.getAssociativity(temp) == "L"))) {
        result.push_back(stack.top());
        stack.pop();
    }
    stack.push(temp);
}


void VariableManager::setParam(const std::vector<std::string>& postfixExpression) {
    for (const std::string& token : postfixExpression) {
        if (entityQualities.isLetter(token[0]) && variables.find(token) == variables.end() &&
            token != "y" && token != "Y" && token != "x" && token != "X") {
            double value;
            //std::cout << "¬ведите значение дл€ переменной " << token << ": ";
            while (!(std::cin >> value)) {
              //  std::cin.clear();
               // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             //   std::cout << "¬ведите корректное значение: ";
            }
            variables[token] = value;
        }
    }
}
const std::map<std::string, double>& VariableManager::getVariables() const {
    return variables;
}
std::string Parser::correctInfixExpressionBySpacebars(const std::string& infixExpression) {
   std:: string correctExpression;
    auto iter = infixExpression.begin();
    while (iter != infixExpression.end()) {
        auto current = iter;
        auto next = std::next(iter);

        correctExpression.push_back(*current);

        if (next != infixExpression.end()) {
            if (entityQualities.isNumber(*current) && !entityQualities.isNumber(*next) && *next != ' ')
                correctExpression.push_back(' ');

            if (!entityQualities.isNumber(*current) && !entityQualities.isLetter(*current) && *current != ' ')
                correctExpression.push_back(' ');

            if (!entityQualities.isNumber(*current) && entityQualities.isLetter(*current) && !entityQualities.isLetter(*next) && *next != ' ')
                correctExpression.push_back(' ');

            if (!entityQualities.isNumber(*current) && entityQualities.isLetter(*current) && entityQualities.isLetter(*next) && (*next == 'x' || *next == 'X' || entityQualities.isNumber(*next)))
                correctExpression.push_back(' ');

            if ((*current == 'x' || *current == 'X') && (entityQualities.isNumber(*next) || entityQualities.isLetter(*next)))
                correctExpression += " * ";
        }

        ++iter;
    }

    return correctExpression += ' ';
}



Evaluator::Evaluator(Math& mathInstance) : functionsMath(mathInstance) {}
double Evaluator::evaluatePostfix(const std::vector<std::string>& postfix,  std::map<std::string, double>& variables) {
        std::stack<std::string> stackOfOperands;
        for (const std::string& token : postfix) {
            if (!entityQualities.isOperator(token))
                stackOfOperands.push(token);

            else {
                if (token == "#") {
                    std::string negativeOperand = stackOfOperands.top();
                    stackOfOperands.pop();
                    if(!entityQualities.isLetter(negativeOperand.back()))
                        stackOfOperands.push(std::to_string(-stod(negativeOperand)));
                    else {
                        negativeOperand = "#" + negativeOperand;
                        stackOfOperands.push(negativeOperand);
                    }
                }
                else {
                    std::string secondOperand = "1";
                    std::string firstOperand = "0";
                    if (entityQualities.isBinaryOperator(token)) {
                        secondOperand = stackOfOperands.top();
                        stackOfOperands.pop();
                        firstOperand = stackOfOperands.top();
                        stackOfOperands.pop();
                    }
                    else {
                        firstOperand = stackOfOperands.top();
                        stackOfOperands.pop();
                    }
                    stackOfOperands.push(functionsMath.doMath(firstOperand, secondOperand, token, variables));
                }
            }
        }
        return stod(stackOfOperands.top());
    }
/*void Evaluator::handleOperatorEvaluation(const std::string& token, std::stack<std::string>& stackOfOperands, std::map<std::string, double>& variables) {
    if (token == "#") {
        double negativeOperand = stod(stackOfOperands.top());
        stackOfOperands.pop();
        stackOfOperands.push(-negativeOperand);
    }
    else {
        double secondOperand = stackOfOperands.top();
        stackOfOperands.pop();
        double firstOperand = (entityQualities.isBinaryOperator(token)) ? stackOfOperands.top() : 1;
        if (entityQualities.isBinaryOperator(token)) {
            stackOfOperands.pop();
        }
        std::string evaluated = functionsMath.doMath(std::to_string(firstOperand), std::to_string(secondOperand), token, variables);
        stackOfOperands.push(stod(evaluated));
    }
}
*/
