#pragma once
#include <QString>
#include <QMessageBox>

// Базовый класс для всех исключений сниффера
class SnifferException {
public:
    enum Severity { Warning, Error, Critical }; // Уровни серьезности ошибки

    // Конструктор принимает сообщение и уровень серьезности
     SnifferException(const QString& message, Severity severity = Error)
        : message_(message), severity_(severity) {}

    virtual ~SnifferException() = default;

    // Получить текст сообщения об ошибке
    QString getMessage() const { return message_; }

    // Получить уровень серьезности ошибки
    Severity getSeverity() const { return severity_; }

private:
    QString message_;  // Сообщение об ошибке
    Severity severity_; // Уровень серьезности
};

// Исключение, связанное с ошибками сокета
class SocketException : public SnifferException {
public:
     SocketException(const QString& message, Severity severity = Error)
        : SnifferException("Socket error: " + message, severity) {}
};

// Исключение, связанное с ошибками файлов
class FileException : public SnifferException {
public:
     FileException(const QString& message, Severity severity = Error)
        : SnifferException("File error: " + message, severity) {}
};

// Исключение, связанное с процессом захвата пакетов
class CaptureException : public SnifferException {
public:
     CaptureException(const QString& message, Severity severity = Error)
        : SnifferException("Capture error: " + message, severity) {}
};

// Исключение, связанное с некорректным вводом данных
class InputException : public SnifferException {
public:
     InputException(const QString& message, Severity severity = Error)
        : SnifferException("Input error: " + message, severity) {}
};
