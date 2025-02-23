#include "Pipeline.h"

std::string CLASS_NAME = "Pipeline";


void Pipeline::stripUnicode(std::string & str) // Remove all non-ascii characters
{ 
    str.erase(
        std::remove_if(
            str.begin(),
            str.end(),
            [](char c){ return !(c >= 0 && c < 128 ); }
        ),
        str.end()
    );  
}


void Pipeline::toLowerCase(std::string& str) { // Make all characters lowercase
    std::transform(
        str.begin(),
        str.end(),
        str.begin(),
        [](unsigned char c) { return std::tolower(c); }
    );
}


void Pipeline::removeSpecial(std::string& str) { // Remove all special characters from the string (&, *, ^, etc.)
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


void Pipeline::removeExtraSpaces(std::string& str) { // Remove double spaces left in string
    // Remove tabs and newlines
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

    // Remove double spaces
    str.erase(
        std::unique(
            str.begin(),
            str.end(),
            [](char lhs, char rhs) { return lhs == ' ' && rhs == ' '; }
        ),
        str.end()
    );
}


void Pipeline::run(std::string & original) {
    stripUnicode(original);
    toLowerCase(original);
    removeSpecial(original);
    removeExtraSpaces(original);
}