//BasePacketFilter.h
#pragma once
#include "PacketFilter.h"

class BasePacketFilter : public PacketFilter {
protected:
    PacketFilter* nextFilter;

public:
    BasePacketFilter() : nextFilter(nullptr) {}

    void SetNextFilter(PacketFilter* nextFilter) override {
        this->nextFilter = nextFilter;
    }

    bool PassToNextFilter(const IPHeader* ipHeader) {
        if (nextFilter) {
            return nextFilter->Filter(ipHeader);
        }
        return true;  // Если нет следующего фильтра, пакет проходит
    }
};
