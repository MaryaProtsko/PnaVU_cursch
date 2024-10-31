#pragma once
#ifndef HOTEL_H
#define HOTEL_H

#include <vector>
#include <string>
#include "Floor.h"

//  ласс, представл€ющий гостиницу
class Hotel {
    std::vector<Floor> floors;

public:
    Hotel();
    void displayAvailableRooms(int floorNumber, int numGuests);
    bool bookRoom(int floorNumber, int roomNumber, const std::vector<std::shared_ptr<Guest>>& guests);
    bool evictGuests(int floorNumber, int roomNumber);
    bool transferGuests(int fromFloor, int fromRoom, int toFloor, int toRoom);
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};

#endif
