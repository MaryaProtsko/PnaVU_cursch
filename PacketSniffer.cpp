#include "PacketSniffer.h"
#include <mstcpip.h>
#include <QFile>
#include "SnifferExceptions.h"
#include "UDPPacket.h"
#include "TCPPacket.h"

// Конструктор инициализирует сокет
PacketSniffer::PacketSniffer(CaptureWindow* captureWindow)
    : captureWindow_(captureWindow){
    WSADATA wsaData;
    int startupResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // Инициализация Winsock
    if (startupResult != 0) {
        throw SocketException(QString("WSAStartup failed with error: %1").arg(startupResult));
    }

    socket_ = socket(AF_INET, SOCK_RAW, IPPROTO_IP); // Создаем сырой сокет
    if (socket_ == INVALID_SOCKET) {
        throw SocketException(QString("Failed to create socket. Error: %1").arg(WSAGetLastError()));
    }
}

// Деструктор закрывает сокет и освобождает ресурсы Winsock
PacketSniffer::~PacketSniffer() {
    closesocket(socket_);
    WSACleanup();
}

// Устанавливает интерфейсный IP-адрес для привязки сокета
void PacketSniffer::SetInterfaceIP(const std::string& ip) {
        closesocket(socket_); // Закрываем старый сокет перед повторной инициализацией
        socket_ = socket(AF_INET, SOCK_RAW, IPPROTO_IP); // Создаем новый сокет

        sockaddr_in socketAddress_;
        socketAddress_.sin_family = AF_INET;

         // Преобразуем IP-адрес из строки в числовой формат
        if (my_inet_pton(AF_INET, ip.c_str(), &socketAddress_.sin_addr) != 1) {
            throw SocketException(QString("Invalid IP address format: %1").arg(QString::fromStdString(ip)));
        }

        // Привязываем сокет к интерфейсному IP-адресу
        if (bind(socket_, (SOCKADDR*)&socketAddress_, sizeof(SOCKADDR)) == SOCKET_ERROR) {
            throw SocketException(QString("Bind failed with error: %1").arg(WSAGetLastError()));
        }

        emit serviceMessage(QString("Socket successfully bound to interface %1").arg(QString::fromStdString(ip)));
}

// Включает или выключает промискуитетный режим на сокете
void PacketSniffer::SetPromiscuousMode(int flag) {
    u_long mode = flag;
    if (ioctlsocket(socket_, SIO_RCVALL, &mode) != 0) {
        throw SocketException(QString("Failed to set promiscuous mode. Error: %1").arg(WSAGetLastError()));
    }
    else {
        emit serviceMessage("Promiscuous mode set successfully.");
    }
}

// Запускает процесс захвата пакетов
void PacketSniffer::Run() {
    capturing = true;
    SetPromiscuousMode(1); // Включаем промискуитетный режим
    emit serviceMessage(QString("Starting capture..."));

    while (capturing) {
        try {
            CapturePacket(); // Захватываем пакеты в цикле
        }
        catch (SnifferException& ex) {
            captureWindow_->handleException(ex);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Небольшая пауза для уменьшения нагрузки
    }

    SetPromiscuousMode(0);  // Отключаем промискуитетный режим при завершении
}

// Останавливает процесс захвата пакетов
void PacketSniffer::StopCapture() {
    try {
        std::lock_guard<std::mutex> lock(mtx); // Защищаем данные мьютексом
        capturing = false; // Ставим флаг завершения
        emit serviceMessage(QString("Capturing is stopped."));
        if (snifferThread.joinable()) {
            snifferThread.join(); // Завершаем поток
        }
    } catch (const std::system_error& ex) {
        throw CaptureException(QString("Failed to stop capture: %1").arg(ex.what()));
    }
}

// Устанавливает цепочку фильтров для обработки пакетов
void PacketSniffer::SetFilterChain(std::unique_ptr<PacketFilter> filter) {
    if (capturing) {
        throw CaptureException("Stop the capture to change filters.", SnifferException::Warning);
    }
    this->filterChain = std::move(filter); // Устанавливаем цепочку фильтров
}

// Захватывает один пакет с использованием сырого сокета
void PacketSniffer::CapturePacket() {
        std::lock_guard<std::mutex> lock(mtx); // Синхронизация доступа
        char buffer[65536]; // Буфер для хранения данных пакета

        fd_set fds;  // Создаем набор дескрипторов сокетов
        FD_ZERO(&fds);  // Инициализируем набор, очищая его
        FD_SET(socket_, &fds);  // Добавляем наш сокет в набор дескрипторов

        timeval timeout;  // Структура для задания таймаута
        timeout.tv_sec = 1;  // Устанавливаем таймаут в 1 секунду (целые секунды)
        timeout.tv_usec = 0;  // Устанавливаем миллисекундную часть таймаута на 0

        // Функция select проверяет, готов ли сокет к чтению
        // Если данные поступили, она возвращает количество готовых дескрипторов
        // В противном случае возвращается 0 (таймаут) или -1 (ошибка)
        int result = select(0, &fds, nullptr, nullptr, &timeout);  // Ждем данных на сокете
        if (result > 0) {
            int bytesReceived = recv(socket_, buffer, sizeof(buffer), 0); // Читаем данные из сокета
            if (bytesReceived == SOCKET_ERROR) {
                throw CaptureException(QString("Failed to receive packet. Error: %1").arg(WSAGetLastError()), SnifferException::Critical);
            }

            if (bytesReceived > 0) {
                IPHeader* ipHeader = reinterpret_cast<IPHeader*>(buffer); // Интерпретируем данные как IP-заголовок

                if (filterChain && !filterChain->Filter(ipHeader)) {
                    return;  // Если пакет не прошел фильтры, пропускаем
                }

                std::unique_ptr<IPPacket> packet = Helper::CreatePacket(ipHeader);

                unsigned short sourcePort = packet->GetSourcePort();
                unsigned short destPort = packet->GetDestinationPort();
                std::string protocolName = packet->GetProtocol();

                if (!packet) {
                    return; // Пропускаем неподдерживаемые протоколы
                }

                // Форматирование времени
                auto now = std::time(nullptr);
                char timeBuffer[100];
                std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

                QString timeStr = QString::fromUtf8(timeBuffer);
                QString sourceIP = QString::fromUtf8(ipHeader->GetSourceIP());
                QString destIP = QString::fromUtf8(ipHeader->GetDestinationIP());
                QString protocolStr = QString::fromStdString(protocolName);

                // Используем std::ostringstream для получения строкового представления пакета
                std::ostringstream packetStream;
                if (packet) {
                    packetStream << *packet;  // Вызывается перегруженный оператор <<
                }

                // Добавляем содержимое пакета в HEX формате
                Helper::DisplayPacketDataInHex(buffer, bytesReceived, packetStream);

                // Получаем результат перегрузки как строку
                QString packetData = QString::fromStdString(packetStream.str());

                // Передаем данные о пакете через сигнал
                emit packetCaptured(timeStr, sourceIP, sourcePort, destIP, destPort, protocolStr, packetData);

                // Сохраняем информацию о пакете в файл
                std::ostringstream packetInfo;
                packetInfo << "Timestamp: " << timeBuffer << "\n";
                Helper::DisplayPacketDataInHex(buffer, bytesReceived, packetInfo);
                // Добавляем полную информацию о пакете в список capturedPackets
                capturedPackets.push_back(packetInfo.str());
            }
            else if (result == SOCKET_ERROR) {
                throw  CaptureException(QString("Select failed with error: %1").arg(WSAGetLastError()), SnifferException::Critical);
            }
        }
}

// Сохраняет текущую сессию захвата в файл
void PacketSniffer::SaveSession(const QTableWidget* table) {
    std::lock_guard<std::mutex> lock(mtx);  // Блокируем мьютекс для синхронизации потоков

    QFile file("session.txt"); // Открываем файл для записи
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // Выбрасываем исключение, если файл не удалось открыть
        throw FileException(QString("Failed to open session.txt for writing: %1").arg(file.errorString()));
    }

    QTextStream out(&file); // Создаем поток для записи в файл

    // Проходим по всем строкам таблицы
    for (int row = 0; row < table->rowCount(); ++row) {
        QStringList rowData; // Список для хранения данных строки

       // Сохраняем данные всех столбцов текущей строки
        for (int col = 0; col < table->columnCount(); ++col) {
            QTableWidgetItem* item = table->item(row, col);
            rowData.append(item ? item->text() : ""); // Если ячейка пустая, добавляем пустую строку
        }

        // Записываем данные строки в файл, разделяя столбцы символом "|"
        out << rowData[0] << "|" // Время
            << rowData[1] << "|" // Источник
            << rowData[2] << "|" // Назначение
            << rowData[3] << "|" // Протокол
            << rowData[4] << "|" // Порт отправителя
            << rowData[5] << "|" // Порт получателя
            << rowData[6].replace("\n", "|") << "\n"; // Сохраняем содержимое пакета, заменяя "\n" на "|"
    }

    file.close(); // Закрываем файл
    emit serviceMessage("Session saved to session.txt."); // Отправляем сообщение об успешном сохранении  
}

// Загружает сохраненную сессию захвата из файла
QStringList PacketSniffer::LoadSession() {
        std::lock_guard<std::mutex> lock(mtx);  // Защита доступа
        QFile file("session.txt"); // Открываем файл для чтения
        QStringList loadedPackets; // Список для хранения загруженных строк

        // Если захват активен, выбрасываем исключение
        if (capturing) {
            throw FileException("Stop the capture to load the session.", SnifferException::Warning);
        }

        // Выбрасываем исключение, если файл не удалось открыть
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw FileException(QString("Failed to open session.txt for reading: %1").arg(file.errorString()), SnifferException::Error);
        }

        QTextStream in(&file); // Создаем поток для чтения из файла
        while (!in.atEnd()) {
            QString line = in.readLine(); // Считываем строку из файла
            if (!line.isEmpty()) {
                loadedPackets.append(line); // Добавляем строки в список
            }
        }

        file.close();  // Закрываем файл
        emit serviceMessage("Session successfully loaded from session.txt.");
        return loadedPackets; // Возвращаем список загруженных строк

}
