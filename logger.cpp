#include "logger.h"

Logger::Logger(const std::string& filename) {
    logFile.open(filename, std::ios::app);
    if (logFile.is_open()) {
        std::time_t now = std::time(nullptr);
        std::string timestamp = std::ctime(&now);
        timestamp.pop_back();
        logFile << "=== Session started: " << timestamp << " ===\n";
        logFile.flush();
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        std::time_t now = std::time(nullptr);
        std::string timestamp = std::ctime(&now);
        timestamp.pop_back();
        logFile << "=== Session ended: " << timestamp << " ===\n\n";
        logFile.close();
    }
}

void Logger::logCommand(const std::string& command) {
    if (logFile.is_open()) {
        logFile << "COMMAND: " << command << "\n";
        logFile.flush();
    }
}

void Logger::logUserInput(const std::string& input) {
    if (logFile.is_open()) {
        logFile << input << "\n";
        logFile.flush();
    }
}