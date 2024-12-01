#pragma once
#include"Header.h"


class IMathCommand {
public:
    virtual ~IMathCommand() = default;
    virtual double execute(double x1, double x2) const = 0;
};

class Equal : public IMathCommand {
public:
    double execute(double x1, double x2) const override {
        return x1;
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
            return std::numeric_limits<double>::quiet_NaN(); // Handle division by zero
        return x1 / x2;
    }
};

class Power : public IMathCommand {
public:
    double execute(double x1, double x2) const override {
        if (x1 < 0 && fmod(x2, 1) != 0)
            return -pow(-x1, x2);// Handle negative base with non-integer exponent
        return pow(x1, x2);  // Standard power calculation
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
        if (x1 < -1 || x1 > 1)
            return std::numeric_limits<double>::quiet_NaN(); // Invalid input for arcsin
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
        if (x1 < -1 || x1 > 1)
            return std::numeric_limits<double>::quiet_NaN(); // Invalid input for arccos
        return acos(x1);
    }
};

class Tangens : public IMathCommand {
public:
    double execute(double x1, double) const override {
        double nearestAsymptote = M_PI_2 + M_PI * std::round((x1 - M_PI_2) / M_PI);
        if (std::fabs(x1 - nearestAsymptote) < 1e-3)
            return std::numeric_limits<double>::quiet_NaN(); // Handle value near asymptote
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
        double nearestAsymptote = M_PI * std::round((x1 - M_PI_2) / M_PI);
        if (std::fabs(x1 - nearestAsymptote) < 1e-3)
            return std::numeric_limits<double>::quiet_NaN(); // Handle value near asymptote
        return 1.0 / tan(x1);
    }
};

class Arccotangens : public IMathCommand {
public:
    double execute(double x1, double) const override {
        return M_PI / 2 - atan(x1);
    }
};

class SquareRoot : public IMathCommand {
public:
    double execute(double x1, double) const override {
        if (x1 < 0)
            return std::numeric_limits<double>::quiet_NaN(); // Square root of a negative number is invalid
        return sqrt(x1);
    }
};

class Root : public IMathCommand {
public:
    double execute(double x1, double x2) const override {
        if (x1 == 0)
            return std::numeric_limits<double>::quiet_NaN(); // Root degree cannot be zero

        if (x2 < 0 && static_cast<int>(x1) != x1)
            return std::numeric_limits<double>::quiet_NaN(); // Cannot take a non-integer root of a negative number

        if (x2 > 0)
            return pow(x2, 1.0 / x1); // Valid root calculation
        else
            return std::numeric_limits<double>::quiet_NaN(); // Invalid root if the second operand is non-positive

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
        if (x1 <= 0 || x1 == 1 || x2 <= 0)
            return std::numeric_limits<double>::quiet_NaN(); // Invalid input for logarithm
        return log(x2) / log(x1);
    }
};

class DecimalLog : public IMathCommand {
public:
    double execute(double x1, double) const override {
        if (x1 <= 0)
            return std::numeric_limits<double>::quiet_NaN(); // Logarithm of non-positive number is invalid
        return log10(x1);
    }
};

class NaturalLog : public IMathCommand {
public:
    double execute(double x1, double) const override {
        if (x1 <= 0)
            return std::numeric_limits<double>::quiet_NaN(); // Logarithm of non-positive number is invalid
        return log(x1);
    }
};


class Precedence {
public:
    Precedence();

    int getPrecedence(const std::string& operatorToFind);

private:
    std::map<std::string, int> precedenceMap;
};

class Associativity {
public:
    Associativity();

    std::string getAssociativity(const std::string& operatorToGetAssociativity);

private:
    std::map<std::string, std::string> associativityMap;
};


class MathTable: public Operators {
public:
    MathTable();
    std::map<std::string, std::shared_ptr<IMathCommand>>& getMathTable() {
        return mathTable;
    }
private:
    std::map<std::string, std::shared_ptr<IMathCommand>> mathTable;

    std::vector<std::shared_ptr<IMathCommand>> commands;
};

class Math {
public:
    static Math& getInstance() {
        static Math instance;
        return instance;
    }

    std::string doMath(std::string firstPotentialOperand, std::string secondPotentialOperand, std::string operation, std::map<std::string, double>& literalVariablesValuesMap);
    double parseOperand(const std::string& potentialOperand, const std::map<std::string, double>& literalVariablesValuesMap);
    int getPrecedence(const std::string& operatorToFind) {
       return precedenceMap.getPrecedence(operatorToFind);
    }
    std::string getAssociativity(const std::string& operatorToGetAssociativity){
        return associativityMap.getAssociativity(operatorToGetAssociativity);
    }
    std::map<std::string, std::shared_ptr<IMathCommand>>& getMathTable() {
        return mathTable.getMathTable();
    }
private:

    MathTable mathTable;
    Precedence precedenceMap;
    Associativity associativityMap;

    Math() : mathTable(), precedenceMap(), associativityMap() {}
    ~Math() = default;

    Math(const Math&) = delete;
    Math& operator=(const Math&) = delete;
   
};


