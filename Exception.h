#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <exception>

class Exception : public std::exception {
public:
    Exception(std::string _str, long _lineNumber = -1)
        : str(_str), lineNumber(_lineNumber) {}

    void showMessage() const {
        std::cout << "Unknown  : " << str << " [line:" << lineNumber << "]"
                  << std::endl;
    }

private:
    std::string str;
    int lineNumber;
};

#endif
