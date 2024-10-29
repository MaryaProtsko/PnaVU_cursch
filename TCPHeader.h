// TCPHeader.h
#pragma once
#ifndef TCP_HEADER_H
#define TCP_HEADER_H

struct TCPHeader {
    unsigned short sourcePort;   // ���� ���������
    unsigned short destPort;     // ���� ����������
    unsigned int sequence;       // ���������� �����
    unsigned int ackSequence;    // ����� �������������
    unsigned char dataOffset;    // ����� ���������
    unsigned char flags;         // ����� ����������
    unsigned short window;       // ������ ����
    unsigned short checksum;     // ����������� �����
    unsigned short urgentPointer; // ��������� �� ������� ������
};

#endif