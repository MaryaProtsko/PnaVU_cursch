#pragma once
#include <WinSock2.h>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <ws2tcpip.h>
#include "SnifferUtils.h"

class IPHeader {
public:
    unsigned char ver_len;         // Версия и длина заголовка.
    unsigned char tos;             // Тип обслуживания.
    unsigned short length;         // Общая длина пакета.
    unsigned short id;             // Идентификатор пакета.
    unsigned short flag_offset;    // Флаги и смещение.
    unsigned char ttl;             // Время жизни (TTL).
    unsigned char protocol;        // Протокол.
    unsigned short checksum;       // Контрольная сумма.
    unsigned int src;              // IP-адрес источника.
    unsigned int dest;             // IP-адрес назначения.

    // Метод для получения строкового представления IP-адреса источника
    const char* GetSourceIP() const {
        static char srcIP[INET_ADDRSTRLEN];
        my_inet_ntop(AF_INET, &src, srcIP, INET_ADDRSTRLEN); // Преобразуем числовой IP в строку
        return srcIP;
    }

    // Метод для получения строкового представления IP-адреса назначения
    const char* GetDestinationIP() const {
        static char destIP[INET_ADDRSTRLEN];
        my_inet_ntop(AF_INET, &dest, destIP, INET_ADDRSTRLEN); // Преобразуем числовой IP в строку
        return destIP;
    }
};
