#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>
#include "Hotel.h"
#include "Guest.h"

// Функция для отображения меню
void displayMenu() {
    std::cout << "\nМеню:\n"
        << "1. Показать доступные номера\n"
        << "2. Забронировать номер\n"
        << "3. Выселить гостей\n"
        << "4. Переселить гостей\n"
        << "5. Сохранить состояние гостиницы в файл\n"
        << "6. Загрузить состояние гостиницы из файла\n"
        << "7. Выйти из программы\n";
}

int main() {
    SetConsoleOutputCP(1251);
    Hotel hotel;
    hotel.loadFromFile("hotel_data.txt"); // Загрузка состояния гостиницы из файла
    std::cout << "Файл успешно загружен...";

    while (true) {
        displayMenu();
        int choice;
        std::cout << "Ваш выбор: ";
        std::cin >> choice;

        try {
            if (choice == 1) { // Показать доступные номера
                int floorNumber, numGuests;
                std::cout << "Введите номер этажа (1-5): ";
                std::cin >> floorNumber;
                std::cout << "Введите количество гостей: ";
                std::cin >> numGuests;
                hotel.displayAvailableRooms(floorNumber, numGuests);
            }
            else if (choice == 2) { // Забронировать номер
                int floorNumber, numGuests;
                std::cout << "Введите номер этажа (1-5): ";
                std::cin >> floorNumber;
                std::cout << "Введите количество гостей: ";
                std::cin >> numGuests;

                std::vector<std::shared_ptr<Guest>> guests;
                for (int i = 0; i < numGuests; ++i) {
                    std::string name;
                    int age;
                    std::cout << "Введите имя гостя " << i + 1 << ": ";
                    std::cin >> name;
                    std::cout << "Введите возраст гостя " << i + 1 << ": ";
                    std::cin >> age;
                    guests.emplace_back(std::make_shared<Guest>(name, age));
                }

                std::cout << "Доступные номера на этаже " << floorNumber << ":\n";
                hotel.displayAvailableRooms(floorNumber, numGuests);
                int roomNumber;
                std::cout << "Выберите номер для заселения: ";
                std::cin >> roomNumber;

                if (!hotel.bookRoom(floorNumber, roomNumber, guests)) {
                    std::cout << "Заселение не удалось. Попробуйте снова.\n";
                }
            }
            else if (choice == 3) { // Выселить гостей
                int floorNumber, roomNumber;
                std::cout << "Введите номер этажа (1-5): ";
                std::cin >> floorNumber;
                std::cout << "Введите номер комнаты для выселения: ";
                std::cin >> roomNumber;

                if (!hotel.evictGuests(floorNumber, roomNumber)) {
                    std::cout << "Выселение не удалось. Комната пуста или не найдена.\n";
                }
            }
            else if (choice == 4) { // Переселить гостей
                int fromFloor, fromRoom, toFloor, toRoom;
                std::cout << "Введите этаж и номер комнаты для переселения: ";
                std::cin >> fromFloor >> fromRoom;
                std::cout << "Введите этаж и номер целевой комнаты: ";
                std::cin >> toFloor >> toRoom;

                if (!hotel.transferGuests(fromFloor, fromRoom, toFloor, toRoom)) {
                    std::cout << "Переселение не удалось.\n";
                }
            }
            else if (choice == 5) { // Сохранить состояние гостиницы
                hotel.saveToFile("hotel_data.txt");
                std::cout << "Состояние гостиницы сохранено.\n";
            }
            else if (choice == 6) { // Загрузить состояние гостиницы
                hotel.loadFromFile("hotel_data.txt");
                std::cout << "Состояние гостиницы загружено.\n";
            }
            else if (choice == 7) { // Выход
                std::cout << "Выход из программы...\n";
                break;
            }
            else {
                std::cout << "Неверный выбор. Попробуйте снова.\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << "\n";
        }
    }

    return 0;
}
