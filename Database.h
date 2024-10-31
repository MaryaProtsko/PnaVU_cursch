#ifndef DATABASE_H
#define DATABASE_H

#include <QString>

class Database
{
public:
    static void initDatabase();
    static void executeQuery(const QString& query);
    static bool userExists(const QString& email);
    static bool validateUser(const QString& email, const QString& password);
};

#endif // DATABASE_H
