#include <QApplication>
#include <QMessageBox.h>
#include "CaptureWindow.h"
#include "SnifferExceptions.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // Инициализация Qt-приложения
    try {
        CaptureWindow window; // Создаем главное окно приложения
        QIcon appIcon("resources/icon.png");
        app.setWindowIcon(appIcon);
        window.show(); // Отображаем окно
        return app.exec(); // Запускаем главный цикл приложения
    } catch (const SnifferException& ex) {
         // Обрабатываем пользовательские исключения
        QMessageBox::critical(nullptr, "Critical Error", ex.getMessage());
    } catch (const std::exception& ex) {
        // Обрабатываем стандартные исключения
        QMessageBox::critical(nullptr, "Critical Error", QString("Unhandled exception: %1").arg(ex.what()));
    } catch (...) {
        // Обрабатываем неизвестные исключения
        QMessageBox::critical(nullptr, "Critical Error", "An unknown error occurred.");
    }

    return -1; // Возвращаем -1 в случае ошибки
}
