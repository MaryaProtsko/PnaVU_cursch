#pragma once
#include "IPHeader.h"

// Абстрактный базовый класс для фильтров пакетов
class PacketFilter {
private:
    PacketFilter* nextFilter = nullptr; // Указатель на следующий фильтр
public:
    // Метод для установки следующего фильтра в цепочке
    void SetNextFilter(PacketFilter* next) {
        nextFilter = next;
    }

    // Передает пакет следующему фильтру, если он установлен
    bool PassToNextFilter(const IPHeader* ipHeader) const {
        if (nextFilter) {
            return nextFilter->Filter(ipHeader);
        }
        return true;  // Если нет следующего фильтра, пакет проходит
    }

    // Чисто виртуальный метод для фильтрации пакетов
    virtual bool Filter(const IPHeader* ipHeader) const = 0;

    virtual ~PacketFilter() = default; // Виртуальный деструктор
};
