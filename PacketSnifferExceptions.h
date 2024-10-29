// PacketSnifferExceptions.h
#pragma once
#include <stdexcept>

class SnifferException : public std::runtime_error {
public:
    explicit SnifferException(const std::string& message) : std::runtime_error(message) {}
};

class SocketException : public SnifferException {
public:
    explicit SocketException(const std::string& message) : SnifferException("Socket error: " + message) {}
};

class FilterException : public SnifferException {
public:
    explicit FilterException(const std::string& message) : SnifferException("Filter error: " + message) {}
};
