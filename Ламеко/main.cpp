
#include "mainwidget.h"

#include <QApplication>


void connectToDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/aseislr/Databases/Course.db");

    if (!db.open())
    {
        QMessageBox::critical(nullptr, "Database Error", db.lastError().text());

    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainWidget w;
    w.show();

    connectToDatabase();

    return a.exec();
}
