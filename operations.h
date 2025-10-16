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
std::vector<int> findPipesByName(const std::map<int, Pipe>& pipes, const std::string& name);
std::vector<int> findPipesByStatus(const std::map<int, Pipe>& pipes, bool status);
void batchEditPipes(std::map<int, Pipe>& pipes, const std::vector<int>& pipeIds, Logger& logger);

void addCompressStation(std::map<int, CompressStation>& stations, Logger& logger);
void editCompressStation(std::map<int, CompressStation>& stations, Logger& logger); 
std::vector<int> findStationsByName(const std::map<int, CompressStation>& stations, const std::string& name);
std::vector<int> findStationsByUnusedPercentage(const std::map<int, CompressStation>& stations, double minPercent, double maxPercent = 100.0);

void viewAllObjects(const std::map<int, Pipe>& pipes, const std::map<int, CompressStation>& stations);

#endif