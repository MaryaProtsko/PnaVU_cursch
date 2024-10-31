#include <sqlite3.h>
#include <QDebug>

#include "Database.h"

void Database::initDatabase()
{
    executeQuery("CREATE TABLE IF NOT EXISTS Users ("
                 "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "    name TEXT NOT NULL,"
                 "    surname TEXT NOT NULL,"
                 "    email TEXT NOT NULL UNIQUE,"
                 "    password TEXT NOT NULL"
                 ");");
}

void Database::executeQuery(const QString& query)
{
    sqlite3 *db;
    char *errorMessage = nullptr;
    if (sqlite3_open("system.db", &db) != SQLITE_OK) {
        qDebug() << "Failed to open database: " << sqlite3_errmsg(db);
        sqlite3_close(db);
        return;
    }

    if (sqlite3_exec(db, query.toStdString().c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        qDebug() << "SQL error: " << errorMessage;
        sqlite3_free(errorMessage);
    }
    sqlite3_close(db);
}

bool Database::userExists(const QString& email)
{
    sqlite3 *db;
    if (sqlite3_open("system.db", &db) != SQLITE_OK) {
        qDebug() << "Failed to open database: " << sqlite3_errmsg(db);
        sqlite3_close(db);
        return false;
    }

    QString query = QString("SELECT COUNT(*) FROM Users WHERE email = '%1';").arg(email);

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, query.toStdString().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Failed to fetch user data: " << sqlite3_errmsg(db);
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, email.toStdString().c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        if (sqlite3_column_int(stmt, 0) > 0) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return true;
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return false;
}

bool Database::validateUser(const QString& email, const QString& password) {
    sqlite3 *db;
    if (sqlite3_open("system.db", &db) != SQLITE_OK) {
        qDebug() << "Failed to open database: " << sqlite3_errmsg(db);
        sqlite3_close(db);
        return false;
    }

    QString query = QString("SELECT * FROM users WHERE email = '%1' AND password = '%2';")
                      .arg(email).arg(password);

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.toStdString().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        bool result = (sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return result;
    } else {
        qDebug() << "SQL error: " << sqlite3_errmsg(db);
        sqlite3_close(db);
        return false;
    }
}
