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
    Log() : _classname("Logger") {};
    Log(std::string classname) : _classname(classname) {};
    void error(std::string msg);
    void warning(std::string msg);
    void normal(std::string msg);
private:
    std::string _classname;
};
#endif