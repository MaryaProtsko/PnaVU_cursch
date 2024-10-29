//ProtocolFilter.h
#pragma once
#include "BasePacketFilter.h"

class ProtocolFilter : public BasePacketFilter {
private:
    unsigned char protocol;

public:
    ProtocolFilter(unsigned char protocol) : protocol(protocol) {}

    bool Filter(const IPHeader* ipHeader) override {
        if (ipHeader->protocol == protocol || protocol == 0) {
            return PassToNextFilter(ipHeader);  // Пропускаем дальше по цепочке
        }
        return false;
    }
};
