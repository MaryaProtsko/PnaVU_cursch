#include "User.h"

void User::setName(const QString& name)
{
    this->name = name;
}

void User::setSurname(const QString& surname)
{
    this->surname = surname;
}

void User::setEmail(const QString& email)
{
    this->email = email;
}

QString User::getName() const
{
    return this->name;
}

QString User::getSurname() const
{
    return this->surname;
}

QString User::getEmail() const
{
    return this->email;
}
