#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <string>
#include <map>
#include "pipe.h"
#include "CompressStation.h"
#include "logger.h"
#include "globals.h"

// Константы для тегов
const std::string PIPE_START_TAG = "<PIPE>";
const std::string PIPE_END_TAG = "</PIPE>";
const std::string STATION_START_TAG = "<STATION>";
const std::string STATION_END_TAG = "</STATION>";
const std::string COUNTERS_TAG = "<COUNTERS>";
const std::string COUNTERS_END_TAG = "</COUNTERS>";

void saveData(const std::string& filename, const std::map<int, Pipe>& pipes,
    const std::map<int, CompressStation>& stations, Logger& logger);
void loadData(const std::string& filename, std::map<int, Pipe>& pipes,
    std::map<int, CompressStation>& stations, Logger& logger);

#endif