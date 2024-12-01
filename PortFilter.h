#pragma once
#include "PacketFilter.h"
#include "Helper.h"

// Класс для фильтрации пакетов по порту
class PortFilter : public PacketFilter {
private:
    unsigned short filterPort; // Порт для фильтрации

public:
    PortFilter(unsigned short port) : filterPort(port) {}

    // Метод фильтрации пакетов по порту
    bool Filter(const IPHeader* ipHeader) const override {
        auto packet = Helper::CreatePacket(ipHeader);  // Создаем объект пакета

        if (packet) {
            // Проверяем, соответствуют ли порты фильтру
            if (filterPort == 0 || packet->GetSourcePort() == filterPort || packet->GetDestinationPort() == filterPort) {
                return PassToNextFilter(ipHeader);
            }
        }
        return false;  // Отклоняем пакет, если порты не совпадают
    }
};
