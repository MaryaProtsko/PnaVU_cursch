/*
Основной файл с параметрами призывника. Возмжно потом еще добавлю но это не точно)
*/

#ifndef CONSCRIPT_H
#define CONSCRIPT_H
#include "person.h"
#include <QString>


class Conscript : public Person
{
private:
    QString status;
    QString unit;

public:
    Conscript(const QString& login, const QString& password, const QString& name, const QString& birth_year,
              const QString& adress, const QString& role, const QString& status, const QString& unit);

    QString get_status() const;
    QString get_unit() const;

    void set_status(const QString& status);
    void set_unit(const QString& unit);
};

#endif // CONSCRIPT_H
