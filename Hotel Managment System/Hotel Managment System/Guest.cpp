#include "Guest.h"

// ���������� ������������
Guest::Guest(const std::string& name, int age) : name(name), age(age) {}

// ���������� ������ getName()
std::string Guest::getName() const {
    return name;
}
