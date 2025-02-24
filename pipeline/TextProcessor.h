// TextProcessor.h
#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <string>

namespace TextProcessor {
    void stripUnicode(std::string & str);
    void toLowerCase(std::string& str);
    void removeSpecial(std::string& str);
    void removeExtraSpaces(std::string& str);
    void run(std::string & original);
}

#endif
