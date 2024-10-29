#pragma once
#include"Header.h"

class IMathCommand {
public:
    virtual ~IMathCommand() = default;
    virtual double execute(double x1, double x2) const = 0;
};

class Precedence {
public:
    Precedence();

    int getPrecedence(const std::string& operatorToFind);

private:
    std::unordered_map<std::string, int> precedenceMap;
};

class Associativity {
public:
    Associativity();

    std::string getAssociativity(const std::string& operatorToGetAssociativity);

private:
    std::unordered_map<std::string, std::string> associativityMap;
};


class MathTable {
public:
    MathTable();
    std::unordered_map<std::string, std::unique_ptr<IMathCommand>>getMathTable() {
        return std::move(mathTable);
    }
private:
    std::unordered_map<std::string, std::unique_ptr<IMathCommand>> mathTable;
};

class Math {
public:
    static Math& getInstance() {
        static Math instance;
        return instance;
    }

    std::string doMath(std::string firstPotentialOperand, std::string secondPotentialOperand, std::string operation, std::map<std::string, double>& literalVariablesValuesMap);
    int getPrecedence(const std::string& operatorToFind) {
       return precedenceMap.getPrecedence(operatorToFind);
    }
    std::string getAssociativity(const std::string& operatorToGetAssociativity){
        return associativityMap.getAssociativity(operatorToGetAssociativity);
    }
    std::unordered_map<std::string, std::unique_ptr<IMathCommand>>getMathTable() {
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


 