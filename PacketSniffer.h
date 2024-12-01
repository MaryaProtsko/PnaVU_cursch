#pragma once
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <chrono>
#include <ctime>
#include <sstream>
#include <fstream>
#include "IPHeader.h"
#include "TCPHeader.h"
#include "UDPHeader.h"
#include "Helper.h"
#include "PacketFilter.h"
#include <QObject>
#include <QTableWidget>
#include "CaptureWindow.h"

// Класс для захвата пакетов
class PacketSniffer: public QObject {
    Q_OBJECT

private:
    CaptureWindow* captureWindow_;             // Ссылка на объект CaptureWindow
    SOCKET socket_;                            // Сокет для захвата данных
    bool capturing = false;                    // Флаг состояния захвата
    std::string interfaceIP;                   // IP-адрес интерфейса для захвата
    std::vector<std::string> capturedPackets;  // Список захваченных пакетов
    std::mutex mtx;                            // Мьютекс для синхронизации потоков
    std::thread snifferThread;                 // Поток захвата пакетов
    std::unique_ptr<PacketFilter> filterChain; // Цепочка фильтров для обработки пакетов

public:
    PacketSniffer(CaptureWindow* captureWindow);  // Конструктор
    ~PacketSniffer();                             // Деструктор
    void SetInterfaceIP(const std::string& ip);   // Устанавливает интерфейс для захвата
    void Run();                                   // Запускает процесс захвата
    void StopCapture();                           // Останавливает процесс захвата
    void CapturePacket();                         // Захватывает отдельный пакет
    void SaveSession(const QTableWidget* table);  // Сохраняет данные захвата в файл
    QStringList LoadSession();                    // Загружает данные захвата из файла
    void SetPromiscuousMode(int flag = 1);        // Включает/выключает промискуитетный режим
    void SetFilterChain(std::unique_ptr<PacketFilter> filter);    // Устанавливает цепочку фильтров

signals:
    // Сигнал для передачи данных о пакете
    void packetCaptured(const QString& time, const QString& sourceIP,
                        unsigned short sourcePort, const QString& destIP,
                        unsigned short destPort, const QString& protocol, const QString& packetData);
    // Сигнал для передачи служебных сообщений
    void serviceMessage(const QString& message);
};
