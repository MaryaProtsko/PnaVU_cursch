#include "CaptureWindow.h"
#include <QHeaderView>
#include <PortFilter.h>
#include <ProtocolFilter.h>
#include <qmessagebox.h>
#include "PacketSniffer.h"

CaptureWindow::CaptureWindow(QWidget *parent) : QWidget(parent) {
    // Создаем объект PacketSniffer через std::make_unique
    sniffer = std::make_unique<PacketSniffer>(this);

    // Создание элементов интерфейса
    protocolComboBox = new QComboBox(this);  // Выпадающий список для выбора протокола
    protocolComboBox->addItem("Все");
    protocolComboBox->addItem("TCP");
    protocolComboBox->addItem("UDP");

    portLineEdit = new QLineEdit(this); // Поле для ввода номера порта
    portLineEdit->setPlaceholderText("Введите номер порта");

    ipLineEdit = new QLineEdit(this); // Поле для ввода IP-адреса
    ipLineEdit->setPlaceholderText("Введите IP-адрес");

    startButton = new QPushButton("Запуск захвата", this); // Кнопка для запуска сниффера
    stopButton = new QPushButton("Остановить захват", this);// Кнопка для остановки
    stopButton->setEnabled(false); // Изначально кнопка отключена

    saveButton = new QPushButton("Сохранить результаты", this); // Кнопка для сохранения данных
    loadButton = new QPushButton("Загрузить сессию", this); // Кнопка для загрузки сохраненной сессии

    // Таблица для отображения информации о пакетах
    packetTable = new QTableWidget(this);
    packetTable->setColumnCount(7); // Устанавливаем количество столбцов
    QStringList headers = {"Время", "Источник", "Порт отправителя", "Назначение", "Порт получателя", "Протокол"};
    packetTable->setHorizontalHeaderLabels(headers);
    packetTable->setColumnHidden(6, true);  // Скрываем последний столбец с данными пакета
    packetTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // Запрещаем редактирование
    packetTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  // Растягиваем на всю ширину окна

    // Поле для содержимого пакета
    packetContent = new QTextEdit(this);
    packetContent->setReadOnly(true); // Только для чтения

    // Поле для служебной информации
    serviceLog = new QTextEdit(this);
    serviceLog->setReadOnly(true);

    // Лейауты для организации элементов
    QVBoxLayout *mainLayout = new QVBoxLayout(this); // Главный вертикальный лейаут
    QHBoxLayout *filterLayout = new QHBoxLayout; // Лейаут для фильтров
    QHBoxLayout *contentLayout = new QHBoxLayout; // Лейаут для содержимого пакетов

    filterLayout->addWidget(protocolComboBox);
    filterLayout->addWidget(portLineEdit);
    filterLayout->addWidget(ipLineEdit);

    mainLayout->addLayout(filterLayout);
    mainLayout->addWidget(startButton);
    mainLayout->addWidget(stopButton);
    mainLayout->addWidget(saveButton);
    mainLayout->addWidget(loadButton);

    mainLayout->addWidget(packetTable);
    setLayout(mainLayout);

    contentLayout->addWidget(packetContent);
    contentLayout->addWidget(serviceLog);

    mainLayout->addLayout(contentLayout);

    // Связь кнопок с методами
    connect(startButton, &QPushButton::clicked, this, &CaptureWindow::startCapture);
    connect(stopButton, &QPushButton::clicked, this, &CaptureWindow::stopCapture);
    connect(saveButton, &QPushButton::clicked, this, &CaptureWindow::saveCapture);
    connect(loadButton, &QPushButton::clicked, this, &CaptureWindow::loadCapture);

    // Связь таблицы с методом для отображения содержимого пакета
    connect(packetTable, &QTableWidget::cellClicked, this, &CaptureWindow::displayPacketContent);

     // Подключение сигналов PacketSniffer к слотам окна
    connect(sniffer.get(), &PacketSniffer::packetCaptured, this, &CaptureWindow::addPacketToTable);
    connect(sniffer.get(), &PacketSniffer::serviceMessage, this, &CaptureWindow::handleServiceMessage);
}

// Метод для добавления информации о пакете в таблицу
void CaptureWindow::addPacketToTable(const QString& time, const QString& sourceIP, unsigned short sourcePort,
                                     const QString& destIP, unsigned short destPort, const QString& protocol,
                                     const QString& packetData){
    int row = packetTable->rowCount(); // Получаем текущий индекс новой строки
    packetTable->insertRow(row); // Вставляем новую строку

     // Устанавливаем данные в соответствующие ячейки
    packetTable->setItem(row, 0, new QTableWidgetItem(time));
    packetTable->setItem(row, 1, new QTableWidgetItem(sourceIP));
    packetTable->setItem(row, 2, new QTableWidgetItem(QString::number(sourcePort)));  // Порт отправителя
    packetTable->setItem(row, 3, new QTableWidgetItem(destIP));
    packetTable->setItem(row, 4, new QTableWidgetItem(QString::number(destPort)));    // Порт получателя
    packetTable->setItem(row, 5, new QTableWidgetItem(protocol));

    // Сохраняем содержимое пакета в скрытый столбец
    packetTable->setItem(row, 6, new QTableWidgetItem(packetData));
}

// Метод для отображения содержимого пакета при выборе строки таблицы
void CaptureWindow::displayPacketContent(int row, int /*column*/) {
    QString packetData = packetTable->item(row, 6)->text();// Берем данные из скрытого столбца
    packetContent->setText(packetData);  // Отображаем данные в текстовом поле
}

// Обработка служебных сообщений и добавление их в текстовое поле
void CaptureWindow::handleServiceMessage(const QString& message) {    
    // Добавление сообщения в текстовое поле
    serviceLog->append(message);
}

// Метод запуска захвата пакетов
void CaptureWindow::startCapture() {
    try {
        // Получение IP, протокола и порта для фильтрации
        QString ipAddress = ipLineEdit->text();
        QString protocol = protocolComboBox->currentText();

        // Проверка порта
        unsigned short port = 0; // Значение по умолчанию, отключающее фильтрацию по порту
        if (!portLineEdit->text().isEmpty()) {
            bool ok;
            port = portLineEdit->text().toUShort(&ok);
            if (!ok)
                throw InputException("Port must be a valid number.", SnifferException::Warning);
        }

        // Проверка на пустой IP
        if (ipAddress.isEmpty()) {
            // Вывести сообщение об ошибке или не начинать захват
            throw InputException("IP address cannot be empty.", SnifferException::Warning);
        }

        // Установка IP для сниффера
        sniffer->SetInterfaceIP(ipAddress.toStdString());

       // Устанавливаем цепочку фильтров (протокол -> порт)
        std::unique_ptr<PacketFilter> protocolFilter = std::make_unique<ProtocolFilter>( protocol == "TCP" ? IPPROTO_TCP :
                                                            (protocol == "UDP" ? IPPROTO_UDP : 0));

        // Связываем фильтры в цепочку
        std::unique_ptr<PacketFilter> portFilter = std::make_unique<PortFilter>(port);
        // Устанавливаем следующий фильтр
        protocolFilter->SetNextFilter(portFilter.release());

        // Передаём цепочку в PacketSniffer
       sniffer->SetFilterChain(std::move(protocolFilter));

        // Запуск захвата пакетов в отдельном потоке
        if (!snifferThread.joinable()) {
            snifferThread = std::thread(&PacketSniffer::Run, sniffer.get());
        }

        stopButton->setEnabled(true);
        startButton->setEnabled(false);
        }catch(const SnifferException& ex) {
            handleException(ex);  // Обработка исключений
    }
}

// Метод остановки захвата пакетов
void CaptureWindow::stopCapture() {
    try {
        // Остановка захвата пакетов
        sniffer->StopCapture();
        if (snifferThread.joinable()) {
            snifferThread.join(); // Завершаем поток захвата
        }
        startButton->setEnabled(true);
        stopButton->setEnabled(false);
    } catch (const SnifferException& ex) {
        handleException(ex);
    }
}

// Метод сохранения результатов захвата
void CaptureWindow::saveCapture() {
    // Сохранение результатов в файл
    try {
        sniffer->SaveSession(packetTable);
    }catch (const SnifferException& ex) {
        handleException(ex);
    }
}

// Метод загрузки сохраненной сессии
void CaptureWindow::loadCapture() {
    try {
        QStringList loadedPackets = sniffer->LoadSession();  // Загружаем данные из файла

        packetTable->setRowCount(0); // Очистка таблицы

         // Парсим загруженные строки и добавляем их в таблицу
        for (const QString& packet : loadedPackets) {
            QStringList packetData = packet.split("|");
            if (packetData.size() < 7) continue; // Пропускаем, если строка неполная

            QString packetContent = packetData.mid(6).join("\n"); // Объединяем данные с переводом строки
            addPacketToTable(packetData[0],  // Время
                             packetData[1],  // Источник
                             packetData[2].toUShort(),  // Порт отправителя
                             packetData[3],  // Назначение
                             packetData[4].toUShort(),  // Порт получателя
                             packetData[5],  // Протокол
                             packetContent); // Содержимое пакета
        }
    } catch (const SnifferException& ex) {
        handleException(ex);
    }
}

// Обработка исключений с выводом сообщения
void CaptureWindow::handleException(const SnifferException& ex) {
    QString message = ex.getMessage();
    QMessageBox::Icon icon;

    // Выбор уровня критичности сообщения
    switch (ex.getSeverity()) {
    case SnifferException::Warning:
        icon = QMessageBox::Warning;
        break;
    case SnifferException::Error:
        icon = QMessageBox::Critical;
        break;
    case SnifferException::Critical:
        icon = QMessageBox::Critical;
        break;
    }

    // Показываем сообщение в зависимости от уровня критичности
    QMessageBox msgBox(icon, "Error", message, QMessageBox::Ok, this);
    msgBox.exec();
}

CaptureWindow::~CaptureWindow() = default;
