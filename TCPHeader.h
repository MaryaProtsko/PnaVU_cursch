// TCPHeader.h
#pragma once
#ifndef TCP_HEADER_H
#define TCP_HEADER_H

struct TCPHeader {
    unsigned short sourcePort;   // Порт источника
    unsigned short destPort;     // Порт назначения
    unsigned int sequence;       // Порядковый номер
    unsigned int ackSequence;    // Номер подтверждения
    unsigned char dataOffset;    // Длина заголовка
    unsigned char flags;         // Флаги управления
    unsigned short window;       // Размер окна
    unsigned short checksum;     // Контрольная сумма
    unsigned short urgentPointer; // Указатель на срочные данные
};

#endif