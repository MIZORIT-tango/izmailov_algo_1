#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <ctime>

class Logger {
private:
    std::ofstream logFile;

public:
    Logger(const std::string& filename);
    ~Logger();
    void logCommand(const std::string& command); 
    void logUserInput(const std::string& input); 
};

#endif