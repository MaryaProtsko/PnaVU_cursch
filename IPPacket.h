#pragma once
#include "Packet.h"

// Класс для представления IP-пакета, унаследованного от базового класса Packet
class IPPacket : public Packet {
public:
    // Конструктор с параметрами для задания IP-адресов и портов
    IPPacket(const std::string& srcIP, const std::string& destIP, unsigned short srcPort, unsigned short destPort)
        : Packet(srcIP, destIP, srcPort, destPort) {}

    // Метод для вывода информации о пакете
    void DisplayInfo(std::ostream& os) const override {
        os << "Source IP: " << sourceIP << "\n"; // Вывод IP источника
        os << "Destination IP: " << destinationIP << "\n"; // Вывод IP назначения
    }

     // Получение названия протокола пакета
    std::string GetProtocol() const { return protocol; }

    // Получение порта источника
    unsigned short GetSourcePort() const { return sourcePort; }

    // Получение порта назначения
    unsigned short GetDestinationPort() const { return destinationPort; }

};

// Шаблонная функция для отображения информации о любом типе пакета
template <typename PacketType>
void DisplayPacketInfo(const PacketType& packet, std::ostream& os) {
    packet.IPPacket::DisplayInfo(os); // Вызов метода базового класса
    os << "Protocol: " << packet.GetProtocol() << "\n"
       << "Source Port: " << packet.GetSourcePort() << "\n"
       << "Destination Port: " << packet.GetDestinationPort() << "\n";
}
