#pragma once
#include <string>
#include <cstring>
#include <WinSock2.h>
#include <string>
#include <cstring>
#include "IPHeader.h"
#include "IPPacket.h"
#include "SnifferExceptions.h"
#include "TCPHeader.h"
#include "TCPPacket.h"
#include "UDPHeader.h"
#include "UDPPacket.h"
#include <memory>

// Утилита для преобразования IP-адресов между строковым и числовым форматами

// Функция преобразует числовой IP-адрес в строковый
inline const char* my_inet_ntop(int af, const void* src, char* dst, int size) {
    if (af == AF_INET) {
        const unsigned char* src_bytes = reinterpret_cast<const unsigned char*>(src);
        if (std::snprintf(dst, size, "%u.%u.%u.%u",
                          src_bytes[0], src_bytes[1], src_bytes[2], src_bytes[3]) < 0) {
            throw CaptureException("Failed to convert IP address to string");
        }
        return dst;
    }
    // В нашем случае реализуем только для AF_INET
    throw CaptureException("Address family not supported");
}


// Функция преобразует строковый IP-адрес в числовой (структура in_addr)
inline int my_inet_pton(int af, const char* src, void* dst) {
    if (af == AF_INET) {
        unsigned char dst_bytes[4];
        if (std::sscanf(src, "%hhu.%hhu.%hhu.%hhu", &dst_bytes[0], &dst_bytes[1], &dst_bytes[2], &dst_bytes[3]) != 4) {
            return 0;  // Некорректный IP
        }
        std::memcpy(dst, dst_bytes, sizeof(dst_bytes));
        return 1;  // Успешное преобразование
    }
    // В нашем случае реализуем только для AF_INET
    throw CaptureException("Address family not supported");
}
