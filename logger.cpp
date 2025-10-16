#include "logger.h"
#include <chrono>
#include <iomanip>

Logger::Logger(const std::string& filename) {
    logFile.open(filename, std::ios::app);
    if (logFile.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);

        char timestamp[26];
        ctime_s(timestamp, sizeof(timestamp), &time_t);

        std::string timeStr(timestamp);
        if (!timeStr.empty() && timeStr.back() == '\n') {
            timeStr.pop_back();
        }

        logFile << "=== Session started: " << timeStr << " ===\n";
        logFile.flush();
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);

        char timestamp[26];
        ctime_s(timestamp, sizeof(timestamp), &time_t);

        std::string timeStr(timestamp);
        if (!timeStr.empty() && timeStr.back() == '\n') {
            timeStr.pop_back();
        }

        logFile << "=== Session ended: " << timeStr << " ===\n\n";
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