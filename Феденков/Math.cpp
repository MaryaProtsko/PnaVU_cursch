#define _USE_MATH_DEFINES
#include"MathHeader.h"




std::string Math::doMath(std::string firstPotentialOperand, std::string secondPotentialOperand, std::string operation, std::map<std::string, double>& literalVariablesValuesMap) {
    try {
        // Search for operation in mathTable
        auto operationCurrent = mathTable.getMathTable().find(operation);
        if (operationCurrent == mathTable.getMathTable().end()) {
            throw std::invalid_argument("Unknown operation: " + operation);
        }

        double firstOperand = 0.0, secondOperand = 0.0;

        // First operand parsing
        firstOperand = parseOperand(firstPotentialOperand, literalVariablesValuesMap);

        // Second operand parsing
        secondOperand = parseOperand(secondPotentialOperand, literalVariablesValuesMap);

        // Operation handling
        double result = operationCurrent->second->execute(firstOperand, secondOperand);
        return std::to_string(result);
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error in doMath: " << e.what() << std::endl;
        return "Error";
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return "Error";
    }
}

double Math::parseOperand(const std::string& potentialOperand, const std::map<std::string, double>& literalVariablesValuesMap) {
    bool isNegative = false;
    std::string operand = potentialOperand;

    // Negative number check
    if (!operand.empty() && operand[0] == '#') {
        isNegative = true;
        operand = operand.substr(1); // Extract '#'
    }

    // Variable search in map
    auto it = literalVariablesValuesMap.find(operand);
    if (it != literalVariablesValuesMap.end()) {
        double value = it->second;
        return isNegative ? -value : value;
    }

    // String to double parsing
    try {
        double value = std::stod(operand);
        return isNegative ? -value : value;
    }
    catch (const std::invalid_argument&) {
        throw std::invalid_argument("Invalid operand: " + operand);
    }
    catch (const std::out_of_range&) {
        throw std::out_of_range("Operand out of range: " + operand);
    }
}



Precedence::Precedence() {
        precedenceMap["#"] = 5;
        precedenceMap["^"] = 4;
        //Mostly all functions have the same precedence
        std::vector<std::string> functions = {
            "sin", "cos", "tan", "tg", "cot", "ctg", "arcsin", "asin", "arccos", "acos",
            "arctg", "atg", "arcctg", "actg", "atan", "acot", "log", "ln", "lg",
            "arctan", "arccot", "sqrt","root", "abs"
        };
        for (const auto& func : functions)
            precedenceMap[func] = 3;


        precedenceMap["*"] = 2;
        precedenceMap["/"] = 2;

        precedenceMap["+"] = 1;
        precedenceMap["-"] = 1;
        precedenceMap["="] = 1;
        precedenceMap["("] = 0; // for brackets handling
    }

int Precedence::getPrecedence(const std::string& operatorToFind) {
    auto precedence = precedenceMap.find(operatorToFind);
    if (precedence != precedenceMap.end())
        return precedence->second;

    throw std::invalid_argument("Unknown operator: " + operatorToFind);
}



Associativity::Associativity() {
    std::vector<std::string> functions = {
        "sin", "cos", "tan", "tg", "cot", "ctg", "arcsin", "asin", "arccos", "acos",
        "arctg", "atg", "arcctg", "actg", "atan", "acot", "log", "ln", "lg",
        "arctan", "arccot", "sqrt","root", "abs", "^","#"
    };
    //Initializing rigth associative functions
    for (const auto& func : functions)
        associativityMap[func] = "R";

    std::vector<std::string> leftAssociative = {
        "+", "-", "tan", "*", "/","="
    };
    //Initializing left associative functions
    for (const auto& func : leftAssociative)
        associativityMap[func] = "L";

}

std::string Associativity::getAssociativity(const std::string& operatorToGetAssociativity) {
    auto associativity = associativityMap.find(operatorToGetAssociativity);
    if (associativity != associativityMap.end())
        return associativity->second;

    throw std::invalid_argument("Unknown operator: " + operatorToGetAssociativity);
}

MathTable::MathTable() {
    try{
        //Use of command pattern to create a vector of function pointers with same Interface
        commands.emplace_back(std::make_shared<Equal>());
        commands.emplace_back(std::make_shared<Addition>());
        commands.emplace_back(std::make_shared<Subtraction>());
        commands.emplace_back(std::make_shared<Multiplication>());
        commands.emplace_back(std::make_shared<Division>());
        commands.emplace_back(std::make_shared<Power>());
        commands.emplace_back(std::make_shared<Sinus>());
        commands.emplace_back(std::make_shared<Arcsinus>());
        commands.emplace_back(std::make_shared<Cosinus>());
        commands.emplace_back(std::make_shared<Arccosinus>());
        commands.emplace_back(std::make_shared<Tangens>());
        commands.emplace_back(std::make_shared<Arctangens>());
        commands.emplace_back(std::make_shared<Cotangens>());
        commands.emplace_back(std::make_shared<Arccotangens>());
        commands.emplace_back(std::make_shared<SquareRoot>());
        commands.emplace_back(std::make_shared<Root>());
        commands.emplace_back(std::make_shared<AbsValue>());
        commands.emplace_back(std::make_shared<LogBase>());
        commands.emplace_back(std::make_shared<DecimalLog>());
        commands.emplace_back(std::make_shared<NaturalLog>());

        for (size_t i = 0; i < commands.size(); i++)
            mathTable[operators[i]] = commands[i];
        }
        catch (const std::exception& e) {
            std::cerr << "Error initializing MathTable: " << e.what() << std::endl;
        }
}
