#pragma once
#ifndef GUEST_H
#define GUEST_H

#include "NamedEntity.h"
#include <string>

//  ласс, представл€ющий гост€
class Guest : public NamedEntity {
public:
    std::string name;
    int age;

    Guest(const std::string& name, int age);
    std::string getName() const override;
};

#endif
