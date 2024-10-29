//CaptureWindow.h
#ifndef CAPTUREWINDOW_H
#define CAPTUREWINDOW_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTextEdit>
#include <thread>
#include "PacketSniffer.h"

class CaptureWindow : public QWidget {
    Q_OBJECT

public:
    CaptureWindow(QWidget *parent = nullptr);

private slots:
    void startCapture();
    void stopCapture();
    void saveCapture();
    void addPacketToTable(QString time, QString sourceIP, QString destIP, QString protocol, unsigned short sourcePort, unsigned short destPort, QString packetData);
    void displayPacketContent(int row, int column);

private:
    QComboBox *protocolComboBox;
    QLineEdit *portLineEdit;
    QLineEdit *ipLineEdit;
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *saveButton;
    QTableWidget *packetTable;
    QTextEdit *packetContent;
    PacketSniffer sniffer;
    std::thread snifferThread;
};

#endif // CAPTUREWINDOW_H
