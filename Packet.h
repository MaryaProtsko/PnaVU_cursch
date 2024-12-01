#pragma once
#include <string>
#include <iostream>

// Абстрактный класс, представляющий общий пакет
class Packet {
protected:
    std::string sourceIP;            // IP-адрес источника.
    std::string destinationIP;       // IP-адрес назначения.
    unsigned short sourcePort;       // Порт источника.
    unsigned short destinationPort;  // Порт назначения.
    std::string protocol;            // Протокол пакета.

public:
    // Конструктор для инициализации основных параметров пакета
    Packet(const std::string& srcIP, const std::string& destIP,
           unsigned short srcPort, unsigned short destPort)
        : sourceIP(srcIP), destinationIP(destIP), sourcePort(srcPort), destinationPort(destPort) {}

     // Чисто виртуальный метод для отображения информации о пакете
    virtual void DisplayInfo(std::ostream& os) const = 0;

    // Перегрузка оператора << для вывода информации о пакете
    friend std::ostream& operator<<(std::ostream& os, const Packet& packet) {
        packet.DisplayInfo(os);  // Вызываем нужный метод DisplayInfo для разных классов
        return os;
    }

    virtual ~Packet() {} // Виртуальный деструктор
};
