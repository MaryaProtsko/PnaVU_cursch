#include "datetimeutils.h"

#include <chrono>
#include <sstream>
#include <iomanip>

std::string get_nowtime_formatted(std::string format){
    const std::time_t now_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream timestream;
    timestream << std::put_time(std::localtime(&now_t), format.c_str());
    return timestream.str();
}
std::string DatetimeUtils::get_nowtime(enum time_format tf){
    switch (tf){
    case FILENAME:
        return get_nowtime_formatted("%d-%m-%Y %H-%M-%S");
        break;
    case STANDART:
        return get_nowtime_formatted("%d.%m.%Y %H:%M:%S");
    default:
        return "undefined time format";
    }
}
