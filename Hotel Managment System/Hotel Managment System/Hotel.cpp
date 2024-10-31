#include "Hotel.h"
#include <fstream>
#include <iostream>

// Конструктор гостиницы
Hotel::Hotel() {
    for (int i = 1; i <= 5; ++i) {
        floors.emplace_back(i);
    }
}

void Hotel::displayAvailableRooms(int floorNumber, int numGuests) {
    if (floorNumber >= 1 && floorNumber <= 5) {
        floors[floorNumber - 1].displayAvailableRooms(numGuests);
    }
    else {
        throw std::out_of_range("Неверный номер этажа.");
    }
}

bool Hotel::bookRoom(int floorNumber, int roomNumber, const std::vector<std::shared_ptr<Guest>>& guests) {
    if (floorNumber >= 1 && floorNumber <= 5) {
        Room* room = floors[floorNumber - 1].findRoom(roomNumber);
        return room && room->addGuests(guests);
    }
    return false;
}

bool Hotel::evictGuests(int floorNumber, int roomNumber) {
    if (floorNumber >= 1 && floorNumber <= 5) {
        Room* room = floors[floorNumber - 1].findRoom(roomNumber);
        if (room) {
            room->clearRoom();
            return true;
        }
    }
    return false;
}

bool Hotel::transferGuests(int fromFloor, int fromRoom, int toFloor, int toRoom) {
    Room* from = floors[fromFloor - 1].findRoom(fromRoom);
    Room* to = floors[toFloor - 1].findRoom(toRoom);
    if (from && to && to->isAvailable(from->getOccupancy())) {
        to->addGuests(from->getGuests());
        from->clearRoom();
        return true;
    }
    return false;
}

void Hotel::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    for (const auto& floor : floors) {
        floor.save(outFile);
    }
}

void Hotel::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (inFile) {
        for (auto& floor : floors) {
            floor.load(inFile);
        }
    }
    else {
        std::cout << "Файл не найден. Создается новая гостиница...\n";
    }
}
