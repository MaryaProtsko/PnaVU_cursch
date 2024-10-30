#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <memory>

// =================== Абстрактный базовый класс NamedEntity ===================
/*
Класс NamedEntity — это абстрактный базовый класс, представляющий любой объект, который имеет имя.
Он содержит чисто виртуальную функцию getName(), которая должна быть реализована в классах-наследниках.
*/
class NamedEntity {
public:
    virtual std::string getName() const = 0; // Чисто виртуальная функция для получения имени
    virtual ~NamedEntity() = default; // Виртуальный деструктор для корректного удаления наследников
};

// =================== Класс Guest (Гость) ===================
/*
Класс Guest — наследник от NamedEntity. Представляет гостя, содержащего имя и возраст.
Реализует функцию getName() для возвращения имени гостя.
*/
class Guest : public NamedEntity {
public:
    std::string name;
    int age;

    // Конструктор, инициализирующий имя и возраст гостя
    Guest(const std::string& name, int age) : name(name), age(age) {}

    // Реализация виртуальной функции getName() для получения имени гостя
    std::string getName() const override { return name; }
};

// =================== Базовый класс Room (Комната) ===================
/*
Класс Room представляет базовую комнату в гостинице. Содержит номер комнаты, вместимость и вектор гостей.
Имеет виртуальные функции для добавления гостей, освобождения комнаты и отображения списка гостей.
*/
class Room {
protected:
    int roomNumber; // Номер комнаты
    int capacity;   // Вместимость комнаты
    std::vector<std::shared_ptr<Guest>> guests; // Список гостей (умные указатели)

public:
    // Конструктор, инициализирующий номер комнаты и её вместимость
    Room(int number, int cap) : roomNumber(number), capacity(cap) {}

    // Проверяет, достаточно ли места в комнате для указанного числа гостей
    virtual bool isAvailable(int numGuests) const {
        return guests.size() + numGuests <= capacity;
    }

    // Добавляет гостей в комнату, если в ней достаточно места
    virtual bool addGuests(const std::vector<std::shared_ptr<Guest>>& newGuests) {
        if (isAvailable(newGuests.size())) {
            guests.insert(guests.end(), newGuests.begin(), newGuests.end());
            return true;
        }
        return false;
    }

    // Освобождает комнату, удаляя всех гостей
    virtual void clearRoom() {
        guests.clear();
    }

    // Возвращает список гостей, проживающих в комнате
    virtual std::vector<std::shared_ptr<Guest>> getGuests() const {
        return guests;
    }

    // Выводит информацию о гостях в комнате
    virtual void displayGuests() const {
        if (guests.empty()) {
            std::cout << "Комната пуста.\n";
        }
        else {
            for (const auto& guest : guests) {
                std::cout << guest->getName() << " (" << guest->age << " лет)\n";
            }
        }
    }

    // Получение номера комнаты
    int getRoomNumber() const { return roomNumber; }

    // Получение вместимости комнаты
    int getCapacity() const { return capacity; }

    // Получение текущего количества гостей в комнате
    int getOccupancy() const { return guests.size(); }

    // Сохраняет состояние комнаты в файл
    virtual void save(std::ofstream& outFile) const {
        outFile << roomNumber << " " << capacity << " " << guests.size() << "\n";
        for (const auto& guest : guests) {
            outFile << guest->getName() << " " << guest->age << "\n";
        }
    }

    // Загружает состояние комнаты из файла
    virtual void load(std::ifstream& inFile) {
        int numGuests;
        inFile >> roomNumber >> capacity >> numGuests;
        guests.clear();
        for (int i = 0; i < numGuests; ++i) {
            std::string name;
            int age;
            inFile >> name >> age;
            guests.push_back(std::make_shared<Guest>(name, age)); // Используем умные указатели
        }
    }

    virtual ~Room() = default; // Виртуальный деструктор для корректного удаления наследников
};

// =================== Производный класс StandardRoom (Обычная комната) ===================
/*
Класс StandardRoom — производный от Room. Представляет стандартную комнату, рассчитанную на 2 человек.
*/
class StandardRoom : public Room {
public:
    StandardRoom(int number) : Room(number, 2) {} // Конструктор инициализирует номер комнаты и вместимость = 2
};

// =================== Производный класс LargeRoom (Большая комната) ===================
/*
Класс LargeRoom — производный от Room. Представляет большую комнату, рассчитанную на 5 человек.
*/
class LargeRoom : public Room {
public:
    LargeRoom(int number) : Room(number, 5) {} // Конструктор инициализирует номер комнаты и вместимость = 5
};

// =================== Класс Floor (Этаж) ===================
/*
Класс Floor представляет этаж гостиницы. Содержит номер этажа и вектор комнат.
Предоставляет методы для отображения доступных комнат, поиска комнаты по номеру и сохранения/загрузки состояния этажа.
*/
class Floor {
    int floorNumber; // Номер этажа
    std::vector<std::unique_ptr<Room>> rooms; // Список комнат (уникальные указатели)

public:
    // Конструктор, инициализирующий этаж с 5 стандартными и 5 большими комнатами
    Floor(int number) : floorNumber(number) {
        for (int i = 1; i <= 10; ++i) {
            if (i <= 5) {
                rooms.push_back(std::make_unique<StandardRoom>(i));
            }
            else {
                rooms.push_back(std::make_unique<LargeRoom>(i));
            }
        }
    }

    // Отображает все доступные комнаты на этаже для заданного числа гостей
    void displayAvailableRooms(int numGuests) const {
        for (const auto& room : rooms) {
            if (room->isAvailable(numGuests)) {
                std::cout << "Комната " << room->getRoomNumber()
                    << " (Вместимость: " << room->getCapacity()
                    << ", Занято: " << room->getOccupancy() << ")\n";
            }
        }
    }

    // Находит комнату по номеру и возвращает указатель на нее
    Room* findRoom(int roomNumber) {
        for (auto& room : rooms) {
            if (room->getRoomNumber() == roomNumber) return room.get();
        }
        return nullptr;
    }

    // Получение номера этажа
    int getFloorNumber() const { return floorNumber; }

    // Сохраняет состояние этажа в файл
    void save(std::ofstream& outFile) const {
        outFile << floorNumber << " " << rooms.size() << "\n";
        for (const auto& room : rooms) {
            room->save(outFile);
        }
    }

    // Загружает состояние этажа из файла
    void load(std::ifstream& inFile) {
        int numRooms;
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
};

// =================== Класс Hotel (Гостиница) ===================
/*
Класс Hotel представляет гостиницу, содержащую 5 этажей.
Обеспечивает функции для бронирования комнаты, выселения гостей, переселения гостей, сохранения и загрузки состояния.
*/
class Hotel {
    std::vector<Floor> floors; // Вектор этажей

public:
    // Конструктор, создающий гостиницу с 5 этажами
    Hotel() {
        for (int i = 1; i <= 5; ++i) {
            floors.emplace_back(i);
        }
    }

    // Отображает доступные комнаты на заданном этаже для указанного числа гостей
    void displayAvailableRooms(int floorNumber, int numGuests) {
        if (floorNumber >= 1 && floorNumber <= 5) {
            floors[floorNumber - 1].displayAvailableRooms(numGuests);
        }
        else {
            throw std::out_of_range("Неверный номер этажа.");
        }
    }

    // Бронирование комнаты, если в ней достаточно места
    bool bookRoom(int floorNumber, int roomNumber, const std::vector<std::shared_ptr<Guest>>& guests) {
        if (floorNumber >= 1 && floorNumber <= 5) {
            Room* room = floors[floorNumber - 1].findRoom(roomNumber);
            return room && room->addGuests(guests);
        }
        return false;
    }

    // Выселение гостей из комнаты
    bool evictGuests(int floorNumber, int roomNumber) {
        if (floorNumber >= 1 && floorNumber <= 5) {
            Room* room = floors[floorNumber - 1].findRoom(roomNumber);
            if (room) {
                room->clearRoom();
                return true;
            }
        }
        return false;
    }

    // Переселение гостей из одной комнаты в другую
    bool transferGuests(int fromFloor, int fromRoom, int toFloor, int toRoom) {
        Room* from = floors[fromFloor - 1].findRoom(fromRoom);
        Room* to = floors[toFloor - 1].findRoom(toRoom);
        if (from && to && to->isAvailable(from->getOccupancy())) {
            to->addGuests(from->getGuests());
            from->clearRoom();
            return true;
        }
        return false;
    }

    // Сохранение состояния гостиницы в файл
    void saveToFile(const std::string& filename) const {
        std::ofstream outFile(filename);
        for (const auto& floor : floors) {
            floor.save(outFile);
        }
    }

    // Загрузка состояния гостиницы из файла
    void loadFromFile(const std::string& filename) {
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
};


// Отображение меню
void displayMenu() {
    std::cout << "Выберите действие:\n";
    std::cout << "1. Посмотреть доступные номера\n";
    std::cout << "2. Заселить гостей\n";
    std::cout << "3. Выселить гостей\n";
    std::cout << "4. Переселить гостей\n";
    std::cout << "5. Сохранить состояние гостиницы\n";
    std::cout << "6. Загрузить состояние гостиницы\n";
    std::cout << "7. Выход\n";
}

int main() {
    Hotel hotel;
    hotel.loadFromFile("hotel_data.txt"); // Загрузка состояния гостиницы из файла

    while (true) {
        displayMenu();
        int choice;
        std::cout << "Ваш выбор: ";
        std::cin >> choice;

        try {
            if (choice == 1) {
                int floorNumber;
                std::cout << "Введите номер этажа (1-5): ";
                std::cin >> floorNumber;
                int numGuests;
                std::cout << "Введите количество гостей: ";
                std::cin >> numGuests;
                hotel.displayAvailableRooms(floorNumber, numGuests);
            }
            else if (choice == 2) {
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
                    guests.emplace_back(std::make_shared<Guest>(name, age)); // Используем умные указатели
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
            else if (choice == 3) {
                int floorNumber, roomNumber;
                std::cout << "Введите номер этажа (1-5): ";
                std::cin >> floorNumber;
                std::cout << "Введите номер комнаты для выселения: ";
                std::cin >> roomNumber;

                if (!hotel.evictGuests(floorNumber, roomNumber)) {
                    std::cout << "Выселение не удалось. Комната пуста или не найдена.\n";
                }
            }
            else if (choice == 4) {
                int fromFloor, fromRoom, toFloor, toRoom;
                std::cout << "Введите этаж и номер комнаты для переселения: ";
                std::cin >> fromFloor >> fromRoom;
                std::cout << "Введите этаж и номер целевой комнаты: ";
                std::cin >> toFloor >> toRoom;

                if (!hotel.transferGuests(fromFloor, fromRoom, toFloor, toRoom)) {
                    std::cout << "Переселение не удалось.\n";
                }
            }
            else if (choice == 5) {
                hotel.saveToFile("hotel_data.txt");
            }
            else if (choice == 6) {
                hotel.loadFromFile("hotel_data.txt");
            }
            else if (choice == 7) {
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
