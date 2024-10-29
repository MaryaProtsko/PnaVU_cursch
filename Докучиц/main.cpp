// В данном файле объявляется база данных и само отображение интерфейса

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); // инициалаизация базы данных
    db.setDatabaseName("/Users/cleanair/Documents/DataBaseForCourse/millitary_office.db"); // Путь к базе данных

    if (!db.open()) { // обработка того, что не удалось открыть базу данных по какой-либо ошибке
        qDebug() << "Не удалось открыть базу данных:" << db.lastError().text(); // Вывод сообщения
        return -1; // Завершаем программу
    }

    MainWindow w;
    w.show();
    return a.exec();
}
