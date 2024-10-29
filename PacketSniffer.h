//PacketSniffer.h
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
#include "HexHelper.h"
#include "PacketFilter.h"
#include <QObject>  //для использования сигналов и слотов
class PacketSniffer: public QObject {
    Q_OBJECT

private:
    SOCKET socket_;
    bool capturing = false;
    std::string interfaceIP;
    std::vector<std::string> capturedPackets;
    std::mutex mtx;
    PacketFilter* filterChain = nullptr;  // Добавляем цепочку фильтров

public:
    PacketSniffer();
    ~PacketSniffer();
    void SetInterfaceIP(const std::string& ip);
    void Run();
    void StopCapture();
    void CapturePacket();
    void SaveSession();
    void SetPromiscuousMode(int flag = 1);
    void SetFilterChain(PacketFilter* filter);  // Метод для установки цепочки фильтров
signals:
    void packetCaptured(QString time, QString sourceIP, QString destIP, QString protocol, unsigned short sourcePort, unsigned short destPort, QString packetData);  // Сигнал для передачи данных о пакете

};
