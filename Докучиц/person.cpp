/*
Инициализация всех функций из заголовочного файла
*/
#include "person.h"


Person::Person(const QString& login, const QString& password, const QString& name, const QString& birth_year, const QString& adress, const QString& role)
    : login(login), password(password), name(name), birth_year(birth_year), adress(adress), role(role) {}


QString Person::get_login() const
{
    return this->login;
}

QString Person::get_password() const
{
    return this->password;
}

QString Person::get_name() const
{
    return this->name;
}

QString Person::get_birth_year() const
{
    return this->birth_year;
}

QString Person::get_address() const
{
    return this->adress;
}

QString Person::get_role() const
{
    return this->role;
}


void Person::set_adress(QString &adress)
{
    this->adress = adress;
}
