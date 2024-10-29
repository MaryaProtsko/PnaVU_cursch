#include "Exception.h"
#include <iostream>
#include <cstring>

Exception::Exception() {}
Exception::~Exception() {}

using namespace std;

Exception::Exception(int errorCode, const char *errorMessage)
{
    code = errorCode;
    std::strncpy(this->message, errorMessage, 80);

}
void Exception::show() const {
    std::cout << "\nError code: " << code << "\nError message: " << message << std::endl;
}
