#include "CaptureWindow.h"
#include <QHeaderView>
#include <PortFilter.h>
#include <ProtocolFilter.h>

CaptureWindow::CaptureWindow(QWidget *parent) : QWidget(parent) {
    // Создание элементов интерфейса
    protocolComboBox = new QComboBox(this);
    protocolComboBox->addItem("Все");
    protocolComboBox->addItem("TCP");
    protocolComboBox->addItem("UDP");
    portLineEdit = new QLineEdit(this);
    portLineEdit->setPlaceholderText("Введите номер порта");

    ipLineEdit = new QLineEdit(this);
    ipLineEdit->setPlaceholderText("Введите IP-адрес");

    startButton = new QPushButton("Запуск захвата", this);
    stopButton = new QPushButton("Остановить захват", this);
    saveButton = new QPushButton("Сохранить результаты", this);

    // Таблица для пакетов
    packetTable = new QTableWidget(this);
    packetTable->setColumnCount(7);
    QStringList headers = {"Время", "Источник", "Порт отправителя", "Назначение", "Порт получателя", "Протокол"};
    packetTable->setHorizontalHeaderLabels(headers);
    packetTable->setColumnHidden(6, true);  // Скрываем последний столбец с данными пакета
    packetTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // Запрещаем редактирование
    packetTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  // Растягиваем на всю ширину окна
    // Поле для содержимого пакета
    packetContent = new QTextEdit(this);
    packetContent->setReadOnly(true);

    // Лейауты для организации элементов
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *filterLayout = new QHBoxLayout;

    filterLayout->addWidget(protocolComboBox);
    filterLayout->addWidget(portLineEdit);
    filterLayout->addWidget(ipLineEdit);

    mainLayout->addLayout(filterLayout);
    mainLayout->addWidget(startButton);
    mainLayout->addWidget(stopButton);
    mainLayout->addWidget(saveButton);

    mainLayout->addWidget(packetTable);
    mainLayout->addWidget(packetContent);

    setLayout(mainLayout);

    // Связь кнопок с методами
    connect(startButton, &QPushButton::clicked, this, &CaptureWindow::startCapture);
    connect(stopButton, &QPushButton::clicked, this, &CaptureWindow::stopCapture);
    connect(saveButton, &QPushButton::clicked, this, &CaptureWindow::saveCapture);

    // Связь таблицы с методом для отображения содержимого пакета
    connect(packetTable, &QTableWidget::cellClicked, this, &CaptureWindow::displayPacketContent);

    // Подключаем сигнал packetCaptured к слоту для добавления данных в таблицу
    connect(&sniffer, &PacketSniffer::packetCaptured, this, &CaptureWindow::addPacketToTable);

}

void CaptureWindow::addPacketToTable(QString time, QString sourceIP, QString destIP, QString protocol, unsigned short sourcePort, unsigned short destPort, QString packetData){
    int row = packetTable->rowCount();
    packetTable->insertRow(row);
    packetTable->setItem(row, 0, new QTableWidgetItem(time));
    packetTable->setItem(row, 1, new QTableWidgetItem(sourceIP));
    packetTable->setItem(row, 2, new QTableWidgetItem(QString::number(sourcePort)));  // Порт отправителя
    packetTable->setItem(row, 3, new QTableWidgetItem(destIP));
    packetTable->setItem(row, 4, new QTableWidgetItem(QString::number(destPort)));    // Порт получателя
    packetTable->setItem(row, 5, new QTableWidgetItem(protocol));

    // Сохраняем содержимое пакета для отображения в скрытый столбец
    packetTable->setItem(row, 6, new QTableWidgetItem(packetData));
}

void CaptureWindow::displayPacketContent(int row, int /*column*/) {
    QString packetData = packetTable->item(row, 6)->text();// Берем данные из скрытого столбца
    packetContent->setText(packetData);
}

void CaptureWindow::startCapture() {
    // Получение IP, протокола и порта для фильтрации
    QString ipAddress = ipLineEdit->text();
    QString protocol = protocolComboBox->currentText();
    unsigned short port = portLineEdit->text().toUShort();

    // Проверка на пустой IP
    if (ipAddress.isEmpty()) {
        // Вывести сообщение об ошибке или не начинать захват
        qDebug() << "IP-адрес не может быть пустым.";
        return;
    }

    // Установка IP для сниффера
    sniffer.SetInterfaceIP(ipAddress.toStdString());

    // Установка фильтров
    ProtocolFilter* protocolFilter = new ProtocolFilter( protocol == "TCP" ? IPPROTO_TCP : (protocol == "UDP" ? IPPROTO_UDP : 0));
    PortFilter* portFilter = new PortFilter(port);
    protocolFilter->SetNextFilter(portFilter);
    sniffer.SetFilterChain(protocolFilter);

    // Запуск захвата пакетов в отдельном потоке
    if (!snifferThread.joinable()) {
        snifferThread = std::thread(&PacketSniffer::Run, &sniffer);
    }
}

void CaptureWindow::stopCapture() {
    // Остановка захвата пакетов
    sniffer.StopCapture();
    if (snifferThread.joinable()) {
        snifferThread.join();
    }
}

void CaptureWindow::saveCapture() {
    // Сохранение результатов в файл
    sniffer.SaveSession();
}
