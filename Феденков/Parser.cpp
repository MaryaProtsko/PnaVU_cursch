#include"Parser.h"
#include<QString>
#include<QDialog>
#include<QPushButton>
#include<QLabel>
#include<QVBoxLayout>
#include<QSlider>
#include<QGraphicsTextItem>

std::vector<std::string> Parser::infixToPostfix(const std::string& expression) {
    std::vector<std::string> result;
    std::stack<std::string> stack;
    std::string temp;
    bool lastWasOperator = true;

    for (char ch : expression) {
        if (ch != ' ' && ch != '(' && ch != ')')
            temp.push_back(ch);

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

        if (temp == "-" && lastWasOperator)
            temp = "#"; // Unar minus

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
            token != "y" && token != "Y" && token != "x" && token != "X"
            && token != "r" && token != "R" && token != "theta"&& token != "e" && token != "E"
            && token != "Pi"&& token != "pi" && !entityQualities.isOperator(token) ) {
            double value =getVariableValue(token);
            variables[token] = value;
        }
    }
}


const std::map<std::string, double>& VariableManager::getVariables() const {
    return variables;
}


double VariableManager::getVariableValue(const std::string& variableName, QWidget* parent) {
    QDialog dialog(parent);
    dialog.setWindowTitle(QString("Set parameter %1").arg(QString::fromStdString(variableName)));

    QVBoxLayout layout(&dialog);

    QLabel* label = new QLabel(QString("%1:").arg(QString::fromStdString(variableName)), &dialog);
    QSlider* slider = new QSlider(Qt::Horizontal, &dialog);
    slider->setRange(0, 100);

    QLabel* valueLabel = new QLabel("50", &dialog);
    slider->setValue(50);

    QPushButton* okButton = new QPushButton("OK", &dialog);

    layout.addWidget(label);
    layout.addWidget(slider);
    layout.addWidget(valueLabel);
    layout.addWidget(okButton);

    QObject::connect(slider, &QSlider::valueChanged, [valueLabel](int value) {
        valueLabel->setText(QString::number(value));
    });

    QObject::connect(okButton, &QPushButton::clicked, [&dialog]() {
        dialog.accept();
    });

    dialog.exec();

    return slider->value();
}


std::string Parser::correctInfixExpressionBySpacebars(const std::string& infixExpression) {
    std::string correctExpression;
    auto iter = infixExpression.begin();

    while (iter != infixExpression.end()) {
        std::string current(1, *iter);
        auto next = std::next(iter);
        std::string nextSymbol(1,*next);
        if ((current.find('<') != std::string::npos || current.find('>') != std::string::npos) && nextSymbol!="=" )
            correctExpression.push_back('=');
        else if((current.find('<') != std::string::npos || current.find('>') != std::string::npos) && nextSymbol=="="){
             iter++;
             continue;
        }
        else
            correctExpression.push_back(*iter);


        if (next != infixExpression.end()) {
            if (entityQualities.isNumber(*iter) && !entityQualities.isNumber(*next) && *next != ' ')
                correctExpression.push_back(' ');

            if (!entityQualities.isNumber(*iter) && !entityQualities.isLetter(*iter) && *iter != ' ')
                correctExpression.push_back(' ');

            if (!entityQualities.isNumber(*iter) && entityQualities.isLetter(*iter) && !entityQualities.isLetter(*next) && *next != ' ')
                correctExpression.push_back(' ');

            if (!entityQualities.isNumber(*iter) && entityQualities.isLetter(*iter) && entityQualities.isLetter(*next) && (*next == 'x' || *next == 'X' || entityQualities.isNumber(*next)))
                correctExpression.push_back(' ');

            if ((*iter == 'x' || *iter == 'X') && (entityQualities.isNumber(*next) || entityQualities.isLetter(*next)))
                correctExpression += " * ";
        }

        ++iter;
    }

    return correctExpression += ' ';
}


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
                    std::string secondOperand = "1"; // special values to deal with different types of operators and stack situations
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

