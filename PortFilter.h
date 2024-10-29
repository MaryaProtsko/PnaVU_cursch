//PortFilter.h
#pragma once
#include "BasePacketFilter.h"
#include "TCPHeader.h"
#include "UDPHeader.h"

class PortFilter : public BasePacketFilter {
private:
    unsigned short filterPort;

public:
    PortFilter(unsigned short port) : filterPort(port) {}

    bool Filter(const IPHeader* ipHeader) override {
        unsigned short sourcePort = 0, destPort = 0;

        if (ipHeader->protocol == IPPROTO_TCP) {
            TCPHeader* tcpHeader = reinterpret_cast<TCPHeader*>(
                const_cast<char*>(reinterpret_cast<const char*>(ipHeader)) + (ipHeader->ver_len & 0x0F) * 4
                );
            sourcePort = ntohs(tcpHeader->sourcePort);
            destPort = ntohs(tcpHeader->destPort);
        }
        else if (ipHeader->protocol == IPPROTO_UDP) {
            UDPHeader* udpHeader = reinterpret_cast<UDPHeader*>(
                const_cast<char*>(reinterpret_cast<const char*>(ipHeader)) + (ipHeader->ver_len & 0x0F) * 4
                );
            sourcePort = ntohs(udpHeader->sourcePort);
            destPort = ntohs(udpHeader->destPort);
        }

        if (filterPort == 0 || sourcePort == filterPort || destPort == filterPort) {
            return PassToNextFilter(ipHeader);
        }
        return false;
    }
};