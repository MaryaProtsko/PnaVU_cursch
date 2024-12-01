#pragma once
#include "IPPacket.h"

class UDPPacket : public IPPacket {
public:
    UDPPacket(const std::string& srcIP, const std::string& destIP, unsigned short srcPort, unsigned short destPort)
        : IPPacket(srcIP, destIP, srcPort, destPort) {
        protocol = "UDP";
    }

    void DisplayInfo(std::ostream& os) const override {
        DisplayPacketInfo(*this, os);  // Используем шаблонную функцию
    }
};
