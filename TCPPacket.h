#pragma once
#include "IPPacket.h"

class TCPPacket : public IPPacket {
public:
    TCPPacket(const std::string& srcIP, const std::string& destIP, unsigned short srcPort, unsigned short destPort)
        : IPPacket(srcIP, destIP, srcPort, destPort) {
        protocol = "TCP";
    }

    void DisplayInfo(std::ostream& os) const override {
        DisplayPacketInfo(*this, os);  // Используем шаблонную функцию
    }
};
