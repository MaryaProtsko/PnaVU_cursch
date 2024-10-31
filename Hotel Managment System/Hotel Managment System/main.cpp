#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>
#include "Hotel.h"
#include "Guest.h"

// ������� ��� ����������� ����
void displayMenu() {
    std::cout << "\n����:\n"
        << "1. �������� ��������� ������\n"
        << "2. ������������� �����\n"
        << "3. �������� ������\n"
        << "4. ���������� ������\n"
        << "5. ��������� ��������� ��������� � ����\n"
        << "6. ��������� ��������� ��������� �� �����\n"
        << "7. ����� �� ���������\n";
}

int main() {
    SetConsoleOutputCP(1251);
    Hotel hotel;
    hotel.loadFromFile("hotel_data.txt"); // �������� ��������� ��������� �� �����
    std::cout << "���� ������� ��������...";

    while (true) {
        displayMenu();
        int choice;
        std::cout << "��� �����: ";
        std::cin >> choice;

        try {
            if (choice == 1) { // �������� ��������� ������
                int floorNumber, numGuests;
                std::cout << "������� ����� ����� (1-5): ";
                std::cin >> floorNumber;
                std::cout << "������� ���������� ������: ";
                std::cin >> numGuests;
                hotel.displayAvailableRooms(floorNumber, numGuests);
            }
            else if (choice == 2) { // ������������� �����
                int floorNumber, numGuests;
                std::cout << "������� ����� ����� (1-5): ";
                std::cin >> floorNumber;
                std::cout << "������� ���������� ������: ";
                std::cin >> numGuests;

                std::vector<std::shared_ptr<Guest>> guests;
                for (int i = 0; i < numGuests; ++i) {
                    std::string name;
                    int age;
                    std::cout << "������� ��� ����� " << i + 1 << ": ";
                    std::cin >> name;
                    std::cout << "������� ������� ����� " << i + 1 << ": ";
                    std::cin >> age;
                    guests.emplace_back(std::make_shared<Guest>(name, age));
                }

                std::cout << "��������� ������ �� ����� " << floorNumber << ":\n";
                hotel.displayAvailableRooms(floorNumber, numGuests);
                int roomNumber;
                std::cout << "�������� ����� ��� ���������: ";
                std::cin >> roomNumber;

                if (!hotel.bookRoom(floorNumber, roomNumber, guests)) {
                    std::cout << "��������� �� �������. ���������� �����.\n";
                }
            }
            else if (choice == 3) { // �������� ������
                int floorNumber, roomNumber;
                std::cout << "������� ����� ����� (1-5): ";
                std::cin >> floorNumber;
                std::cout << "������� ����� ������� ��� ���������: ";
                std::cin >> roomNumber;

                if (!hotel.evictGuests(floorNumber, roomNumber)) {
                    std::cout << "��������� �� �������. ������� ����� ��� �� �������.\n";
                }
            }
            else if (choice == 4) { // ���������� ������
                int fromFloor, fromRoom, toFloor, toRoom;
                std::cout << "������� ���� � ����� ������� ��� �����������: ";
                std::cin >> fromFloor >> fromRoom;
                std::cout << "������� ���� � ����� ������� �������: ";
                std::cin >> toFloor >> toRoom;

                if (!hotel.transferGuests(fromFloor, fromRoom, toFloor, toRoom)) {
                    std::cout << "����������� �� �������.\n";
                }
            }
            else if (choice == 5) { // ��������� ��������� ���������
                hotel.saveToFile("hotel_data.txt");
                std::cout << "��������� ��������� ���������.\n";
            }
            else if (choice == 6) { // ��������� ��������� ���������
                hotel.loadFromFile("hotel_data.txt");
                std::cout << "��������� ��������� ���������.\n";
            }
            else if (choice == 7) { // �����
                std::cout << "����� �� ���������...\n";
                break;
            }
            else {
                std::cout << "�������� �����. ���������� �����.\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << "������: " << e.what() << "\n";
        }
    }

    return 0;
}
