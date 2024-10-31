#ifndef FLOOR_H
#define FLOOR_H

#include <vector>
#include <memory>
#include <fstream> // Добавлено для работы с файлами
#include "Room.h"

// Класс, представляющий этаж
class Floor {
    int floorNumber;
    std::vector<std::unique_ptr<Room>> rooms;

public:
    Floor(int number);
    void displayAvailableRooms(int numGuests) const;
    Room* findRoom(int roomNumber);
    int getFloorNumber() const;
    void save(std::ofstream& outFile) const;
    void load(std::ifstream& inFile);
};

#endif
