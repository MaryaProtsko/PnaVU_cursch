#pragma once
#include "IPHeader.h"

#include <iostream>
#include <iomanip>
#include <sstream>

class Helper {
public:

    // Фунция для создания пакета
    static std::unique_ptr<IPPacket> CreatePacket(const IPHeader* ipHeader) {
        if (!ipHeader) {
            return nullptr;  // Проверка на nullptr
        }

        // Извлекаем данные об адресах и портах, определяем протокол
        std::string srcIP = ipHeader->GetSourceIP();
        std::string destIP = ipHeader->GetDestinationIP();
        unsigned short srcPort = 0, destPort = 0;

        if (ipHeader->protocol == IPPROTO_TCP) {
            TCPHeader* tcpHeader = reinterpret_cast<TCPHeader*>(
                reinterpret_cast<char*>(const_cast<IPHeader*>(ipHeader)) + (ipHeader->ver_len & 0x0F) * 4
                );
            srcPort = ntohs(tcpHeader->sourcePort);
            destPort = ntohs(tcpHeader->destPort);
            return std::make_unique<TCPPacket>(srcIP, destIP, srcPort, destPort);
        }
        else if (ipHeader->protocol == IPPROTO_UDP) {
            UDPHeader* udpHeader = reinterpret_cast<UDPHeader*>(
                reinterpret_cast<char*>(const_cast<IPHeader*>(ipHeader)) + (ipHeader->ver_len & 0x0F) * 4
                );
            srcPort = ntohs(udpHeader->sourcePort);
            destPort = ntohs(udpHeader->destPort);
            return std::make_unique<UDPPacket>(srcIP, destIP, srcPort, destPort);
        }

        // Возвращаем nullptr, если протокол не поддерживается
        return nullptr;
    }

    // Функция для записи данных в поток в формате HEX
    static void DisplayPacketDataInHex(const char* data, int length, std::ostringstream& hexData) {
        for (int i = 0; i < length; ++i) {
            // Добавляем новую строку каждые 16 байт
            if (i % 16 == 0) {
                hexData << "\n" << std::hex << std::setw(4) << std::setfill('0') << i << ": ";
            }
            // Выводим каждый байт в HEX формате
            hexData << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)(unsigned char)data[i] << " ";
        }
        hexData << std::dec;  // Возвращаем вывод в десятичный формат
    }
};
