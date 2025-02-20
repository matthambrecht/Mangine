#ifndef PIPELINE_H
#define PIPELINE_H

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>  

class Pipeline {
public:
    Pipeline() {};
    void run(std::string & original);

private:
    std::string CLASS_NAME = "Pipeline";

    void stripUnicode(std::string & str);
    void toLowerCase(std::string & str);
    void removeSpecial(std::string &str);
    void removeExtraSpaces(std::string &str);
};
#endif