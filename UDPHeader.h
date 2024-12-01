#pragma once

struct UDPHeader {
    unsigned short sourcePort;   // Порт источника
    unsigned short destPort;     // Порт назначения
    unsigned short length;       // Длина заголовка и данных
    unsigned short checksum;     // Контрольная сумма
};

