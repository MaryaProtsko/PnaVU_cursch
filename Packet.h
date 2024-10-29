// Packet.h
#pragma once
#include <string>
#include <iostream>

class Packet {
protected:
    std::string sourceIP;
    std::string destinationIP;
    unsigned short sourcePort;
    unsigned short destinationPort;
    std::string protocol;

public:
    Packet(const std::string& srcIP, const std::string& destIP, unsigned short srcPort, unsigned short destPort)
        : sourceIP(srcIP), destinationIP(destIP), sourcePort(srcPort), destinationPort(destPort) {}

    virtual void DisplayInfo(std::ostream& os) const = 0;

    // Перегрузка оператора <<
    friend std::ostream& operator<<(std::ostream& os, const Packet& packet) {
        packet.DisplayInfo(os);  // Вызываем нужный метод DisplayInfo для разных классов
        return os;
    }

    virtual ~Packet() {}
};
