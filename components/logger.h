#ifndef LOGGER_H
#define LOGGER_H

#include <filesystem>
#include <fstream>
#include <string>

enum LogLevel{ DEBUG, INFO, WARNING, ERROR };
extern std::string str_levels[4];

extern std::filesystem::path default_logdir;

class Logger {
private:
    enum LogLevel logfile_min_lvl;
    std::filesystem::path logfile_path;
    std::ofstream logfile_stream = nullptr;
public:
    Logger(enum LogLevel min_log_lvl = INFO);
    ~Logger();
    void log_msg(std::string msg, enum LogLevel lvl = INFO);
};

std::string define_logfile_name();
#endif // LOGGER_H
