#include "Guest.h"

// Реализация конструктора
Guest::Guest(const std::string& name, int age) : name(name), age(age) {}

// Реализация метода getName()
std::string Guest::getName() const {
    return name;
}
