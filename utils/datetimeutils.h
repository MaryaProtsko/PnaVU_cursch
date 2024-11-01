#include <string>

#ifndef DATETIMEUTILS_H
#define DATETIMEUTILS_H

enum time_format{ FILENAME, STANDART };
class DatetimeUtils {
public:
    static std::string get_nowtime(enum time_format tf);
};

#endif // DATETIMEUTILS_H
