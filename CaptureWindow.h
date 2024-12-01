#pragma once
#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTextEdit>
#include <thread>
#include <memory>
#include "SnifferExceptions.h"
class PacketSniffer;

class CaptureWindow : public QWidget {
    Q_OBJECT

public:
    CaptureWindow(QWidget *parent = nullptr);
    ~CaptureWindow();
    void handleException(const SnifferException& ex);

private slots:
    void startCapture();
    void stopCapture();
    void saveCapture();
    void loadCapture();
    void addPacketToTable(const QString& time, const QString& sourceIP, unsigned short sourcePort, const QString& destIP, unsigned short destPort, const QString& protocol, const QString& packetData);
    void displayPacketContent(int row, int column);
    void handleServiceMessage(const QString& message);
private:
    QComboBox *protocolComboBox;
    QLineEdit *portLineEdit;
    QLineEdit *ipLineEdit;
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *saveButton;
    QPushButton *loadButton;
    QTableWidget *packetTable;
    QTextEdit *packetContent;
    QTextEdit *serviceLog;
    std::unique_ptr<PacketSniffer> sniffer;  // Умный указатель
    std::thread snifferThread;
};
