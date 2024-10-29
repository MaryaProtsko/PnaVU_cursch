//IPUtils.h
#pragma once
#include <string>
#include <cstring>
#include <stdexcept>
#include <WinSock2.h>
#include <string>
#include <cstring>
#include <stdexcept>

// Функция inet_ntop преобразует числовой IP-адрес в строковый
 inline const char* my_inet_ntop(int af, const void* src, char* dst, int size) {
    if (af == AF_INET) {
        const unsigned char* src_bytes = reinterpret_cast<const unsigned char*>(src);
        if (std::snprintf(dst, size, "%u.%u.%u.%u",
            src_bytes[0], src_bytes[1], src_bytes[2], src_bytes[3]) < 0) {
            throw std::runtime_error("Failed to convert IP address to string");
        }
        return dst;
    }
    // В нашем случае реализуем только для AF_INET
    throw std::invalid_argument("Address family not supported");
}


// Функция inet_pton преобразует строковый IP-адрес в числовой (структура in_addr)
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
    throw std::invalid_argument("Address family not supported");
}