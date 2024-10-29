//HexHelper.h
#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>

class HexHelper {
public:
    //функция для записи данных в поток
    static void DisplayPacketDataInHex(const char* data, int length, std::ostringstream& hexData) {
        for (int i = 0; i < length; ++i) {
            if (i % 16 == 0) {
                hexData << "\n" << std::hex << std::setw(4) << std::setfill('0') << i << ": ";
            }
            hexData << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)(unsigned char)data[i] << " ";
        }
        hexData << std::dec;  // Возвращаем вывод в десятичный формат
    }
};
