//UDPHeader.h
#pragma once
#ifndef UDP_HEADER_H
#define UDP_HEADER_H

struct UDPHeader {
    unsigned short sourcePort;   // Порт источника
    unsigned short destPort;     // Порт назначения
    unsigned short length;       // Длина заголовка и данных
    unsigned short checksum;     // Контрольная сумма
};

#endif