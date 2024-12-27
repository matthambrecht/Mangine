#include "Log.h"

const std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm tm_time = *std::localtime(&now_time);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(&tm_time, "(%Y-%m-%d %H:%M:%S") << "." 
       << std::setfill('0') << std::setw(3) << ms.count() << ")";

    return ss.str();
}

const std::string formatMsg(
    const std::string& color,
    const std::string& errtype,
    const std::string classname,
    const std::string& msg
    ) {
    return getTimestamp() +
    color + " [" + errtype + "]" + RESET + BOLD_WHITE + 
    " [" + classname + "]" + RESET + " -> " +
    msg;
}

void Log::error(std::string classname, std::string msg) {
   std::cout << formatMsg(RED, "Error", classname, msg) << std::endl;
}

void Log::warning(std::string classname, std::string msg) {
   std::cout << formatMsg(YELLOW, "Warning", classname, msg) << std::endl;
}

void Log::normal(std::string classname, std::string msg) {
   std::cout << formatMsg(GREEN, "Log", classname, msg) << std::endl;
}