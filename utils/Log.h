#ifndef LOG_H
#define LOG_H
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define BOLD_WHITE "\033[1m"
#define RESET   "\033[0m"  

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

class Log {
public:
    Log() {};
    void error(std::string classname, std::string msg);
    void warning(std::string classname, std::string msg);
    void normal(std::string classname, std::string msg);
};
#endif