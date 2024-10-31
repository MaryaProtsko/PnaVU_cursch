#include "Room.h"

// Конструктор класса Room
Room::Room(int number, int cap) : roomNumber(number), capacity(cap) {}

bool Room::isAvailable(int numGuests) const {
    return guests.size() + numGuests <= capacity;
}

bool Room::addGuests(const std::vector<std::shared_ptr<Guest>>& newGuests) {
    if (isAvailable(newGuests.size())) {
        guests.insert(guests.end(), newGuests.begin(), newGuests.end());
        return true;
    }
    return false;
}

void Room::clearRoom() {
    guests.clear();
}

std::vector<std::shared_ptr<Guest>> Room::getGuests() const {
    return guests;
}

void Room::displayGuests() const {
    if (guests.empty()) {
        std::cout << "Комната пуста.\n";
    }
    else {
        for (const auto& guest : guests) {
            std::cout << guest->getName() << " (" << guest->age << " лет)\n";
        }
    }
}

int Room::getRoomNumber() const {
    return roomNumber;
}

int Room::getCapacity() const {
    return capacity;
}

int Room::getOccupancy() const {
    return guests.size();
}

void Room::save(std::ofstream& outFile) const {
    if (outFile) {
        outFile << roomNumber << " " << capacity << " " << guests.size() << "\n";
        for (const auto& guest : guests) {
            outFile << guest->getName() << " " << guest->age << "\n";
        }
    }
}

void Room::load(std::ifstream& inFile) {
    int numGuests;
    if (inFile) {
        inFile >> roomNumber >> capacity >> numGuests;
        guests.clear();
        for (int i = 0; i < numGuests; ++i) {
            std::string name;
            int age;
            inFile >> name >> age;
            guests.push_back(std::make_shared<Guest>(name, age));
        }
    }
}

// Конструктор стандартной комнаты
StandardRoom::StandardRoom(int number) : Room(number, 2) {}

// Конструктор большой комнаты
LargeRoom::LargeRoom(int number) : Room(number, 5) {}
