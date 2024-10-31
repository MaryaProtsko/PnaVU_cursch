#ifndef USER_H
#define USER_H

#include <QString>

class User
{
protected:
    QString name;
    QString surname;
    QString email;

public:
    User(const QString& name, const QString& surname, const QString& email)
        : name(name), surname(surname), email(email) {}

    virtual void displayRole() const = 0;

    void setName(const QString& name);
    void setSurname(const QString& surname);
    void setEmail(const QString& email);
    QString getName() const;
    QString getSurname() const;
    QString getEmail() const;
};

#endif // USER_H
