#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <vector>
#include <memory>
#include <fstream> // ��������� ��� ������ � �������
#include "Guest.h"

// ������� ����� �������
class Room {
protected:
    int roomNumber;
    int capacity;
    std::vector<std::shared_ptr<Guest>> guests;

public:
    Room(int number, int cap);
    virtual bool isAvailable(int numGuests) const;
    virtual bool addGuests(const std::vector<std::shared_ptr<Guest>>& newGuests);
    virtual void clearRoom();
    virtual std::vector<std::shared_ptr<Guest>> getGuests() const;
    virtual void displayGuests() const;
    int getRoomNumber() const;
    int getCapacity() const;
    int getOccupancy() const;
    virtual void save(std::ofstream& outFile) const;
    virtual void load(std::ifstream& inFile);
    virtual ~Room() = default;
};

// ����� ����������� �������
class StandardRoom : public Room {
public:
    StandardRoom(int number);
};

// ����� ������� �������
class LargeRoom : public Room {
public:
    LargeRoom(int number);
};

#endif
