#pragma once
#ifndef NAMEDENTITY_H
#define NAMEDENTITY_H

#include <string>

// Абстрактный базовый класс для объектов с именем
class NamedEntity {
public:
    virtual std::string getName() const = 0; // Чисто виртуальная функция для получения имени
    virtual ~NamedEntity() = default; // Виртуальный деструктор
};

#endif
