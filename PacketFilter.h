//PacketFilter.h
#pragma once
#include "IPHeader.h"

class PacketFilter {
public:
    virtual bool Filter(const IPHeader* ipHeader) = 0;
    virtual void SetNextFilter(PacketFilter* nextFilter) = 0;
    virtual ~PacketFilter() {}
};
