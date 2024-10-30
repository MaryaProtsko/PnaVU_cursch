#include "logger.h"

#include "../utils/datetimeutils.h"

std::filesystem::path default_logdir = "logs/";
std::string str_levels[4] = {"Debug", "Info", "Warning", "Error"};

std::string loglevel_to_str(enum LogLevel lvl){
    return str_levels[(int)lvl];
}

Logger::Logger(enum LogLevel min_log_lvl){
    this->logfile_min_lvl = min_log_lvl;
    this->logfile_path = default_logdir;

    std::string log_filename = DatetimeUtils::get_nowtime(FILENAME) + ".txt";
    this->logfile_path = this->logfile_path.append(log_filename);

    if (!std::filesystem::exists(default_logdir))
        std::filesystem::create_directory(default_logdir);
    this->logfile_stream.open(this->logfile_path, std::ios::out);
    this->logfile_stream << "Log file of XXX program; read carefully\n";
}
Logger::~Logger(){
    this->logfile_stream.close();
}
void Logger::log_msg(std::string msg, enum LogLevel lvl){
    if(this->logfile_stream){
        this->logfile_stream << "["+DatetimeUtils::get_nowtime(STANDART)+"] ";
        this->logfile_stream << "["+loglevel_to_str(lvl)+"] ";
        this->logfile_stream << msg << std::endl;
        std::flush(this->logfile_stream);
    }
}
