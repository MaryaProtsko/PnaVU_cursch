/*
Реализация основных функций из заголовочного
добавить функцию поиска для чего-нибудь!!!
*/
#include "conscript.h"


Conscript::Conscript(const QString& login, const QString& password, const QString& name, const QString& birth_year,
              const QString& adress, const QString& role, const QString& status, const QString& unit)
    : Person(login, password, name, birth_year, adress, role), status(status), unit(unit) {}

QString Conscript::get_status() const
{
    return this->status;
}

QString Conscript::get_unit() const
{
    return this->unit;
}

void Conscript::set_status(const QString& status)
{
    this->status = status;
}

void Conscript::set_unit(const QString& status)
{
    this->unit = unit;
}
