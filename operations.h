#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <map>
#include <vector>
#include "pipe.h"
#include "CompressStation.h"
#include "logger.h"
#include "utils.h"


void addPipe(std::map<int, Pipe>& pipes, Logger& logger);
void editPipe(std::map<int, Pipe>& pipes, Logger& logger);
std::map<int, Pipe> findPipesByName(const std::map<int, Pipe>& pipes, const std::string& name);
std::map<int, Pipe> findPipesByStatus(const std::map<int, Pipe>& pipes, bool status);
std::map<int, Pipe> findPipesById(const std::map<int, Pipe>& pipes, int id);
void batchEditPipes(std::map<int, Pipe>& pipes, Logger& logger);

void addCompressStation(std::map<int, CompressStation>& stations, Logger& logger);
void editCompressStation(std::map<int, CompressStation>& stations, Logger& logger);
std::map<int, CompressStation> findStationsByName(const std::map<int, CompressStation>& stations, const std::string& name);
std::map<int, CompressStation> findStationsByUnusedPercentage(const std::map<int, CompressStation>& stations, double minPercent, double maxPercent = 100.0);
std::map<int, CompressStation> findStationsById(const std::map<int, CompressStation>& stations, int id);

void viewAllObjects(const std::map<int, Pipe>& pipes, const std::map<int, CompressStation>& stations);

#endif