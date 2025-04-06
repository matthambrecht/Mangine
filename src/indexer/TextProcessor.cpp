// TextProcessor.cpp
#include "TextProcessor.h"
#include <algorithm>
#include <cctype>

namespace TextProcessor {
    void stripUnicode(std::string & str) {
        str.erase(
            std::remove_if(
                str.begin(),
                str.end(),
                [](unsigned char c){ return !(static_cast<unsigned char>(c) < 128); }
            ),
            str.end()
        );
    }

    void toLowerCase(std::string& str) {
        std::transform(
            str.begin(),
            str.end(),
            str.begin(),
            [](unsigned char c) { return std::tolower(c); }
        );
    }

    void removeSpecial(std::string& str) {
        str.erase(
            std::remove_if(
                str.begin(),
                str.end(),
                [](char c){
                    return !std::isalnum(static_cast<unsigned char>(c)) && !std::isspace(static_cast<unsigned char>(c));
                }
            ),
            str.end()
        );
    }

    void removeExtraSpaces(std::string& str) {
        std::replace_if(
            str.begin(),
            str.end(),
            [](char c) { return c == '\t' || c == '\n'; },
            ' '
        );

        auto start = str.find_first_not_of(' ');
        if (start == std::string::npos) {
            str.clear();
            return;
        }

        auto end = str.find_last_not_of(' ');
        str = str.substr(start, end - start + 1);

        str.erase(
            std::unique(
                str.begin(),
                str.end(),
                [](char lhs, char rhs) { return lhs == ' ' && rhs == ' '; }
            ),
            str.end()
        );
    }

    void run(std::string & original) {
        stripUnicode(original);
        toLowerCase(original);
        removeSpecial(original);
        removeExtraSpaces(original);
    }
}
