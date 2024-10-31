#include "Floor.h"

// Конструктор класса Floor
Floor::Floor(int number) : floorNumber(number) {
    for (int i = 1; i <= 10; ++i) {
        if (i <= 5) {
            rooms.push_back(std::make_unique<StandardRoom>(i));
        }
        else {
            rooms.push_back(std::make_unique<LargeRoom>(i));
        }
    }
}

void Floor::displayAvailableRooms(int numGuests) const {
    for (const auto& room : rooms) {
        if (room->isAvailable(numGuests)) {
            std::cout << "Комната " << room->getRoomNumber()
                << " (Вместимость: " << room->getCapacity()
                << ", Занято: " << room->getOccupancy() << ")\n";
        }
    }
}

Room* Floor::findRoom(int roomNumber) {
    for (auto& room : rooms) {
        if (room->getRoomNumber() == roomNumber) return room.get();
    }
    return nullptr;
}

int Floor::getFloorNumber() const {
    return floorNumber;
}

void Floor::save(std::ofstream& outFile) const {
    if (outFile) {
        outFile << floorNumber << " " << rooms.size() << "\n";
        for (const auto& room : rooms) {
            room->save(outFile);
        }
    }
}

void Floor::load(std::ifstream& inFile) {
    int numRooms;
    if (inFile) {
        inFile >> floorNumber >> numRooms;
        rooms.clear();
        for (int i = 0; i < numRooms; ++i) {
            std::unique_ptr<Room> room;
            if (i < 5) {
                room = std::make_unique<StandardRoom>(0);
            }
            else {
                room = std::make_unique<LargeRoom>(0);
            }
            room->load(inFile);
            rooms.push_back(std::move(room));
        }
    }
}
