//IPPacket.h
#pragma once
#include "Packet.h"

class IPPacket : public Packet {
public:
    IPPacket(const std::string& srcIP, const std::string& destIP, unsigned short srcPort, unsigned short destPort)
        : Packet(srcIP, destIP, srcPort, destPort) {}

    void DisplayInfo(std::ostream& os) const override {
        os << "Source IP: " << sourceIP << "\n";
        os << "Destination IP: " << destinationIP << "\n";
    }

    std::string GetProtocol() const { return protocol; }
    unsigned short GetSourcePort() const { return sourcePort; }
    unsigned short GetDestinationPort() const { return destinationPort; }

};

template <typename PacketType>
void DisplayPacketInfo(const PacketType& packet, std::ostream& os) {
    packet.IPPacket::DisplayInfo(os);
    os << "Protocol: " << packet.GetProtocol() << "\n"
        << "Source Port: " << packet.GetSourcePort() << "\n"
        << "Destination Port: " << packet.GetDestinationPort() << "\n";
}
