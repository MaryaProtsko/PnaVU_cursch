#define _USE_MATH_DEFINES
#include"MathHeader.h"

class Equal : public IMathCommand {
public:
    double execute(double x1, double x2) const override {
        return x2;
    }
};

class Addition : public IMathCommand {
public:
    double execute(double x1, double x2) const override {
        return x1 + x2;
    }
};

class Subtraction : public IMathCommand {
public:
    double execute(double x1, double x2) const override {
        return x1 - x2;
    }
};

class Multiplication : public IMathCommand {
public:
    double execute(double x1, double x2) const override {
        return x1 * x2;
    }
};

class Division : public IMathCommand {
public:
    double execute(double x1, double x2) const override {
        if (x2 == 0) 
            throw std::invalid_argument("Division by zero");
        return x1 / x2;
    }
};

class Power : public IMathCommand {
public:
    double execute(double x1, double x2) const override {
        return pow(x1, x2);
    }
};

class Sinus : public IMathCommand {
public:
    double execute(double x1, double) const override {
        return sin(x1);
    }
};

class Arcsinus : public IMathCommand {
public:
    double execute(double x1, double) const override {
        if (x1 < -1 || x1>1)
            throw std::invalid_argument("Value Of sin should be between [-1;1]");
        return asin(x1);
    }
};

class Cosinus : public IMathCommand {
public:
    double execute(double x1, double) const override {
        return cos(x1);
    }
};

class Arccosinus : public IMathCommand {
public:
    double execute(double x1, double) const override {
        if (x1 < -1 || x1>1)
            throw std::invalid_argument("Value Of cos should be between [-1;1]");
        return acos(x1);
    }
};

class Tangens : public IMathCommand {
public:
    double execute(double x1, double) const override {
        return tan(x1);
    }
};

class Arctangens : public IMathCommand {
public:
    double execute(double x1, double) const override {
        return atan(x1);
    }
};

class Cotangens : public IMathCommand {
public:
    double execute(double x1, double) const override {
        if (tan(x1) == 0)
            throw std::invalid_argument("Cotangens of PI*n/2 is undefined");
        return 1/tan(x1);
    }
};

class Arccotangens : public IMathCommand {
public:
    double execute(double x1, double) const override {
        return M_PI/2 - atan(x1);
    }
};

class SquareRoot : public IMathCommand {
public:
    double execute(double x1, double) const override {
        if (x1 < 0) {
            throw std::invalid_argument("Square root of a negative number");
        }
        return sqrt(x1);
    }
};

class Root : public IMathCommand {
public:
    double execute(double x1, double x2) const override {
        if (x2 < 0) {
            throw std::invalid_argument("Square root of a negative number");
        }
        return pow(x2,x1);
    }
};

class AbsValue : public IMathCommand {
public:
    double execute(double x1, double) const override {
        return std::abs(x1);
    }
};

class LogBase : public IMathCommand {
public:
    double execute(double x1, double x2) const override {
        if (x2 < 0 || x1 == 1 || x1 <= 0)
            throw std::invalid_argument("Incorrect logBase input");
        return log10(x1) / log10(x2);
    }
};

class DecimalLog : public IMathCommand {
public:
    double execute(double, double x2) const override {
        if (x2 < 0)
            throw std::invalid_argument("Incorrect logBase input");
        return log10(x2);
    }
};

class NaturalLog : public IMathCommand {
public:
    double execute(double, double x2) const override {
        if (x2 < 0)
            throw std::invalid_argument("Incorrect logBase input");
        return log(x2);
    }
};


std::string Math::doMath(std::string firstPotentialOperand, std::string secondPotentialOperand, std::string operation, std::map<std::string, double>& literalVariablesValuesMap) {
    auto operationCurrent = mathTable.getMathTable().find(operation);

   
    if (operationCurrent != mathTable.getMathTable().end()) {
        double firstOperand = 0.0, secondOperand = 0.0;
        bool isNegative1 = false, isNegative2 = false;

       
        if (!firstPotentialOperand.empty() && firstPotentialOperand[0] == '#') {
            isNegative1 = true;
            firstPotentialOperand = firstPotentialOperand.substr(1); // Убираем '#'
        }
        if (literalVariablesValuesMap.find(firstPotentialOperand) != literalVariablesValuesMap.end()) {
            firstOperand = literalVariablesValuesMap[firstPotentialOperand];
            if (isNegative1)
                firstOperand = -firstOperand;
        }
        else {
            firstOperand = stod(firstPotentialOperand);
        }

        if (!secondPotentialOperand.empty() && secondPotentialOperand[0] == '#') {
            isNegative2 = true;
            secondPotentialOperand = secondPotentialOperand.substr(1); // Убираем '#'
        }
        if (literalVariablesValuesMap.find(secondPotentialOperand) != literalVariablesValuesMap.end()) {
            secondOperand = literalVariablesValuesMap[secondPotentialOperand];
            if (isNegative2)
                secondOperand = -secondOperand;
        }
        else {
            secondOperand = stod(secondPotentialOperand);
        }

        auto& command = operationCurrent->second; 
        return std::to_string(command->execute(firstOperand, secondOperand)); 
    }
    else {
        return "1"; 
    }
}



Precedence::Precedence() {
        precedenceMap["#"] = 5;
        precedenceMap["^"] = 4;

        std::vector<std::string> functions = {
            "sin", "cos", "tan", "tg", "cot", "ctg", "arcsin", "asin", "arccos", "acos",
            "arctg", "atg", "arcctg", "actg", "atan", "acot", "log", "ln", "lg",
            "arctan", "arccot", "sqrt","root", "abs"
        };
        for (const auto& func : functions) {
            precedenceMap[func] = 3;
        }

        precedenceMap["*"] = 2;
        precedenceMap["/"] = 2;

        precedenceMap["+"] = 1;
        precedenceMap["-"] = 1;
        precedenceMap["="] = 1;
    }

int Precedence::getPrecedence(const std::string& operatorToFind) {
        auto precedence = precedenceMap.find(operatorToFind);
        if (precedence != precedenceMap.end())
            return precedence->second;
        throw std::invalid_argument("Can't evaluate precedence");
    }


Associativity::Associativity() {
    std::vector<std::string> functions = {
        "sin", "cos", "tan", "tg", "cot", "ctg", "arcsin", "asin", "arccos", "acos",
        "arctg", "atg", "arcctg", "actg", "atan", "acot", "log", "ln", "lg",
        "arctan", "arccot", "sqrt","root", "abs", "^","#"
    };
    for (const auto& func : functions) {
        associativityMap[func] = "R";
    }
    std::vector<std::string> leftAssociative = {
        "+", "-", "tan", "*", "/"
    };
    for (const auto& func : leftAssociative) {
        associativityMap[func] = "L";
    }
}

std::string Associativity::getAssociativity(const std::string& operatorToGetAssociativity) {
    auto associativity = associativityMap.find(operatorToGetAssociativity);
    if (associativity != associativityMap.end())
        return associativity->second;
    throw std::invalid_argument("Incorrect operator can't get associativity");
}




MathTable::MathTable() {
    Operand arrayOfOperands;
    auto equalCommand = std::make_unique<Equal>();
    auto additionCommand = std::make_unique<Addition>();
    auto subtractionCommand = std::make_unique<Subtraction>();
    auto multiplicationCommand = std::make_unique<Multiplication>();
    auto divisionCommand = std::make_unique<Division>();
    auto powerCommand = std::make_unique<Power>();
    auto sinusCommand = std::make_unique<Sinus>();
    auto arcsinusCommand = std::make_unique<Arcsinus>();
    auto cosinusCommand = std::make_unique<Cosinus>();
    auto arccosinusCommand = std::make_unique<Arccosinus>();
    auto tangensCommand = std::make_unique<Tangens>();
    auto arctangensCommand = std::make_unique<Arctangens>();
    auto cotangensCommand = std::make_unique<Cotangens>();
    auto arccotangensCommand = std::make_unique<Arccotangens>();
    auto squareRootCommand = std::make_unique<SquareRoot>();
    auto rootCommand = std::make_unique<Root>();
    auto absValueCommand = std::make_unique<AbsValue>();
    auto logBaseCommand = std::make_unique<LogBase>();
    auto decimalLogCommand = std::make_unique<DecimalLog>();
    auto naturalLogCommand = std::make_unique<NaturalLog>();

    mathTable[arrayOfOperands.operands[0]] = std::move(equalCommand);
    mathTable[arrayOfOperands.operands[1]] = std::move(additionCommand);
    mathTable[arrayOfOperands.operands[2]] = std::move(subtractionCommand);
    mathTable[arrayOfOperands.operands[3]] = std::move(multiplicationCommand);
    mathTable[arrayOfOperands.operands[4]] = std::move(divisionCommand);
    mathTable[arrayOfOperands.operands[5]] = std::move(powerCommand);
    mathTable[arrayOfOperands.operands[6]] = std::move(sinusCommand);
    mathTable[arrayOfOperands.operands[7]] = std::move(arcsinusCommand);
    mathTable[arrayOfOperands.operands[8]] = std::move(arcsinusCommand);
    mathTable[arrayOfOperands.operands[9]] = std::move(cosinusCommand);
    mathTable[arrayOfOperands.operands[10]] = std::move(arccosinusCommand);
    mathTable[arrayOfOperands.operands[11]] = std::move(arccosinusCommand);
    mathTable[arrayOfOperands.operands[12]] = std::move(tangensCommand);
    mathTable[arrayOfOperands.operands[13]] = std::move(tangensCommand);
    mathTable[arrayOfOperands.operands[14]] = std::move(arctangensCommand);
    mathTable[arrayOfOperands.operands[15]] = std::move(arctangensCommand);
    mathTable[arrayOfOperands.operands[16]] = std::move(arctangensCommand);
    mathTable[arrayOfOperands.operands[17]] = std::move(arctangensCommand);
    mathTable[arrayOfOperands.operands[18]] = std::move(cotangensCommand);
    mathTable[arrayOfOperands.operands[19]] = std::move(cotangensCommand);
    mathTable[arrayOfOperands.operands[20]] = std::move(arccotangensCommand);
    mathTable[arrayOfOperands.operands[21]] = std::move(arccotangensCommand);
    mathTable[arrayOfOperands.operands[22]] = std::move(arccotangensCommand);
    mathTable[arrayOfOperands.operands[23]] = std::move(arccotangensCommand);
    mathTable[arrayOfOperands.operands[24]] = std::move(squareRootCommand);
    mathTable[arrayOfOperands.operands[25]] = std::move(rootCommand);
    mathTable[arrayOfOperands.operands[26]] = std::move(absValueCommand);
    mathTable[arrayOfOperands.operands[27]] = std::move(logBaseCommand);
    mathTable[arrayOfOperands.operands[28]] = std::move(decimalLogCommand);
    mathTable[arrayOfOperands.operands[29]] = std::move(naturalLogCommand);

}


