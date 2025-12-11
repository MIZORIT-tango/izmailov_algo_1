#ifndef GASNETWORK_H
#define GASNETWORK_H

#include <map>
#include <vector>
#include <string>
#include "pipe.h"
#include "CompressStation.h"

class GasNetwork {
private:
    std::map<int, Pipe>& pipes;
    std::map<int, CompressStation>& stations;

    std::map<int, std::map<int, int>> adjacencyMatrix; // -1 вход, 1 выход, 0 нет связи
    std::map<int, std::map<int, double>> weightMatrix;

public:
    GasNetwork(std::map<int, Pipe>& p, std::map<int, CompressStation>& s);

    bool createConnection(int startStationId, int endStationId, int diameter);
    void removeStationConnections(int stationId);
    void updateMatrices();
    std::vector<int> topologicalSort();

    void showAdjacencyMatrix();
    void showWeightMatrix();
    void showConnectionsList();

    int findAvailablePipe(int diameter);
    bool canCreateConnection(int startStationId, int endStationId); 

    void calculateAndShowMaxFlow();
    void calculateAndShowShortestPath();
private:
    void dfsTopologicalSort(int stationId, std::vector<bool>& visited, std::vector<int>& result);
    bool hasCycleDFS(int stationId, std::vector<bool>& visited, std::vector<bool>& recursionStack);
    std::vector<int> getVerticesWithNoIncomingEdges();

    double calculateMaxFlow(int sourceId, int sinkId);
    std::pair<double, std::vector<int>> calculateShortestPath(int startId, int endId);
};

#endif