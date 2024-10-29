//PacketSniffer.cpp
#include "PacketSniffer.h"
#include <mstcpip.h>
#include <fstream>
#include "PacketSnifferExceptions.h"
#include "UDPPacket.h"
#include "TCPPacket.h"

PacketSniffer::PacketSniffer() {
    WSADATA wsaData;
    int startupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (startupResult != 0) {
        throw SocketException("WSAStartup failed with error: " + std::to_string(startupResult));
    }
    socket_ = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (socket_ == INVALID_SOCKET) {
        throw SocketException("Failed to create socket. Error: " + std::to_string(WSAGetLastError()));
    }
}

PacketSniffer::~PacketSniffer() {
    closesocket(socket_);
    WSACleanup();
}

void PacketSniffer::SetInterfaceIP(const std::string& ip) {
    // Закрываем старый сокет перед повторной инициализацией
    closesocket(socket_);
    socket_ = socket(AF_INET, SOCK_RAW, IPPROTO_IP);

    sockaddr_in socketAddress_;
    socketAddress_.sin_family = AF_INET;
    if (my_inet_pton(AF_INET, ip.c_str(), &socketAddress_.sin_addr) != 1) {
        throw std::runtime_error("Invalid IP address format");
    }

    if (bind(socket_, (SOCKADDR*)&socketAddress_, sizeof(SOCKADDR)) == SOCKET_ERROR) {
        throw SocketException("Bind failed with error: " + std::to_string(WSAGetLastError()));
    }

    std::cout << "Socket successfully bound to interface " << ip << std::endl;
}

void PacketSniffer::SetPromiscuousMode(int flag) {
    u_long mode = flag;
    if (ioctlsocket(socket_, SIO_RCVALL, &mode) != 0) {
        std::cerr << "Failed to set promiscuous mode. Error: " << WSAGetLastError() << std::endl;
    }
    else {
        std::cout << "Promiscuous mode set successfully." << std::endl;
    }
}

void PacketSniffer::Run() {
    capturing = true;
    SetPromiscuousMode(1);

    while (capturing) {
        try {
            CapturePacket();
        }
        catch (const std::exception& ex) {
            std::cerr << "Error while capturing packet: " << ex.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Небольшая пауза для уменьшения нагрузки
    }

    SetPromiscuousMode(0);  // Отключаем промискуитетный режим при завершении
}

void PacketSniffer::StopCapture() {
    std::lock_guard<std::mutex> lock(mtx);
    capturing = false;
}

void PacketSniffer::SetFilterChain(PacketFilter* filter) {
    this->filterChain = filter;  // Устанавливаем цепочку фильтров
}


void PacketSniffer::CapturePacket() {
    try {
        std::lock_guard<std::mutex> lock(mtx);
        char buffer[65536];

        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(socket_, &fds);

        timeval timeout;
        timeout.tv_sec = 1;  // Устанавливаем таймаут в 1 секунду
        timeout.tv_usec = 0;

        int result = select(0, &fds, nullptr, nullptr, &timeout);
        if (result > 0) {
        int bytesReceived = recv(socket_, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR) {
            throw SocketException("Failed to receive packet. Error: " + std::to_string(WSAGetLastError()));
        }
        if (bytesReceived > 0) {
            IPHeader* ipHeader = reinterpret_cast<IPHeader*>(buffer);
            if (filterChain && !filterChain->Filter(ipHeader)) {
                return;  // Если пакет не прошел фильтры, пропускаем
            }
            unsigned short sourcePort = 0, destPort = 0;
            std::string protocolName;
            std::unique_ptr<Packet> packet;
            // Извлечение портов в зависимости от протокола
            if (ipHeader->protocol == IPPROTO_TCP) {
                TCPHeader* tcpHeader = reinterpret_cast<TCPHeader*>(
                    const_cast<char*>(reinterpret_cast<const char*>(ipHeader)) + (ipHeader->ver_len & 0x0F) * 4
                    );
                sourcePort = ntohs(tcpHeader->sourcePort);
                destPort = ntohs(tcpHeader->destPort);
                protocolName = "TCP";

                // Создаем TCPPacket
                packet = std::make_unique<TCPPacket>(ipHeader->GetSourceIP(), ipHeader->GetDestinationIP(), sourcePort, destPort);
            }
            else if (ipHeader->protocol == IPPROTO_UDP) {
                UDPHeader* udpHeader = reinterpret_cast<UDPHeader*>(
                    const_cast<char*>(reinterpret_cast<const char*>(ipHeader)) + (ipHeader->ver_len & 0x0F) * 4
                    );
                sourcePort = ntohs(udpHeader->sourcePort);
                destPort = ntohs(udpHeader->destPort);
                protocolName = "UDP";

                // Создаем UDPPacket
                packet = std::make_unique<UDPPacket>(ipHeader->GetSourceIP(), ipHeader->GetDestinationIP(), sourcePort, destPort);
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
            HexHelper::DisplayPacketDataInHex(buffer, bytesReceived, packetStream);

            // Получаем результат перегрузки как строку
            QString packetData = QString::fromStdString(packetStream.str());

            // Передаем данные о пакете через сигнал
            emit packetCaptured(timeStr, sourceIP, destIP, protocolStr, sourcePort, destPort, packetData);

            // Сохраняем информацию о пакете в файл
            std::ostringstream packetInfo;
            packetInfo << "Timestamp: " << timeBuffer << "\n";
            HexHelper::DisplayPacketDataInHex(buffer, bytesReceived, packetInfo);
            // Добавляем полную информацию о пакете в список capturedPackets
            capturedPackets.push_back(packetInfo.str());
        }
        else if (result == SOCKET_ERROR) {
            throw SocketException("Select failed with error: " + std::to_string(WSAGetLastError()));
        }
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error while capturing packet: " << ex.what() << std::endl;
    }
}

void PacketSniffer::SaveSession() {
    std::lock_guard<std::mutex> lock(mtx);
    std::ofstream file("session.txt", std::ios::out);
    if (file.is_open()) {
        for (const std::string& packet : capturedPackets) {
            file << packet << "\n";
        }
        file.close();
        std::cout << "Session saved to session.txt.\n";
    }
    else {
        std::cerr << "Failed to save session.\n";
    }
}
