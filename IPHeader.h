//IPHeader.h
#pragma once
#include <WinSock2.h>
#include <iostream>
#include <cstring>
#include <iomanip>
#include "IPUtils.h"

class IPHeader {
public:
    unsigned char ver_len;
    unsigned char tos;
    unsigned short length;
    unsigned short id;
    unsigned short flag_offset;
    unsigned char ttl;
    unsigned char protocol;
    unsigned short checksum;
    unsigned int src;
    unsigned int dest;

    const char* GetSourceIP() const {
        static char srcIP[INET_ADDRSTRLEN];
        my_inet_ntop(AF_INET, &src, srcIP, INET_ADDRSTRLEN);
        return srcIP;
    }

    const char* GetDestinationIP() const {
        static char destIP[INET_ADDRSTRLEN];
        my_inet_ntop(AF_INET, &dest, destIP, INET_ADDRSTRLEN);
        return destIP;
    }
};
