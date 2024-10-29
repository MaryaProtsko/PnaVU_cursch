/*
В данном файле реализована инициализация класса Person для хранения данных пользователя фио дата рождения
адресс
*/
#include <QString>
#ifndef PERSON_H
#define PERSON_H


class Person
{
private:
    QString login;
    QString password;
    QString name;
    QString birth_year;
    QString adress;
    QString role;

public:
    Person(const QString& login, const QString& password, const QString& name, const QString& birth_year, const QString& adress, const QString& role);
    Person() = default;

    QString get_login() const;
    QString get_password() const;
    QString get_name() const;
    QString get_birth_year() const;
    QString get_address() const;
    QString get_role() const;

    void set_adress(QString& adress);

};

#endif // PERSON_H
