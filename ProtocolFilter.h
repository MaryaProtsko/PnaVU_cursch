#pragma once
#include "PacketFilter.h"

// Класс для фильтрации пакетов по протоколу
class ProtocolFilter : public PacketFilter  {
private:
    unsigned char protocol; // Код протокола (TCP, UDP ...)

public:
    // Конструктор с указанием протокола
     ProtocolFilter(unsigned char protocol) : protocol(protocol) {}

    // Метод фильтрации пакетов по протоколу
     bool Filter(const IPHeader* ipHeader) const override {
         if (protocol == 0 || ipHeader->protocol == protocol) {
             return PassToNextFilter(ipHeader); // Пропускаем дальше
         }
         return false; // Отклоняем пакет, если протокол не совпадает
    }
};
