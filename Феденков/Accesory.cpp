#include"Accesory.h"

bool AccesoryChecker::isNumber(char ch) const {
    return std::isdigit(ch) || ch == '.' || ch == ',';
}

bool AccesoryChecker::isLetter(char ch) const {
    return std::isalpha(ch);
}

bool AccesoryChecker::isOperator(const std::string& potentialOperator) const {
    return operatorSet.find(potentialOperator) != operatorSet.end();
}

bool AccesoryChecker::isBinaryOperator(const std::string& potentialBinaryOperator) const {
    static const std::unordered_set<std::string> binaryOperators = { "*", "/", "+", "-", "log", "=", "^" };
    return binaryOperators.find(potentialBinaryOperator) != binaryOperators.end();
}