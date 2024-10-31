#pragma once
#ifndef NAMEDENTITY_H
#define NAMEDENTITY_H

#include <string>

// ����������� ������� ����� ��� �������� � ������
class NamedEntity {
public:
    virtual std::string getName() const = 0; // ����� ����������� ������� ��� ��������� �����
    virtual ~NamedEntity() = default; // ����������� ����������
};

#endif
