//UDPHeader.h
#pragma once
#ifndef UDP_HEADER_H
#define UDP_HEADER_H

struct UDPHeader {
    unsigned short sourcePort;   // ���� ���������
    unsigned short destPort;     // ���� ����������
    unsigned short length;       // ����� ��������� � ������
    unsigned short checksum;     // ����������� �����
};

#endif