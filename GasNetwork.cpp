#include "GasNetwork.h"
#include "utils.h"
#include <iostream>
#include <algorithm>
#include <stack>
#include <set>
#include <queue>

GasNetwork::GasNetwork(std::map<int, Pipe>& p, std::map<int, CompressStation>& s)
    : pipes(p), stations(s) {
    updateMatrices();
}

bool GasNetwork::createConnection(int startStationId, int endStationId, int diameter) {
    if (stations.find(startStationId) == stations.end()) {
        std::cout << "Error: CS with ID " << startStationId << " not found!\n\n\n";
        return false;
    }
    if (stations.find(endStationId) == stations.end()) {
        std::cout << "Error: CS with ID " << endStationId << " not found!\n\n\n";
        return false;
    }

    if (startStationId == endStationId) {
        std::cout << "Mistake: It is impossible to connect the CS with itself!\n\n\n";
        return false;
    }

    int pipeId = findAvailablePipe(diameter);

    if (pipeId == -1) {
        std::cout << "No free pipes found with a diameter of " << diameter << " mm.\n";
        std::cout << "Create a new pipe and try again.\n\n\n";
        return false;
    }

    pipes[pipeId].connect(startStationId, endStationId);
    updateMatrices();

    std::cout << "The connection was created successfully!\n";
    std::cout << "CS" << startStationId << " --pipe#" << pipeId << "--> CS" << endStationId << "\n\n\n";
    return true;
}

int GasNetwork::findAvailablePipe(int diameter) {
    for (auto& pair : pipes) {
        Pipe& pipe = pair.second;
        if (!pipe.getIsConnected() && pipe.getDiameter() == diameter) {
            return pipe.getId();
        }
    }
    return -1;
}

void GasNetwork::removeStationConnections(int stationId) {
    int disconnectedCount = 0;

    for (auto& pair : pipes) {
        Pipe& pipe = pair.second;
        if (pipe.isConnectedToStation(stationId)) {
            pipe.disconnect();
            disconnectedCount++;
        }
    }

    if (disconnectedCount > 0) {
        std::cout << "Connections removed: " << disconnectedCount << "\n\n";
        updateMatrices();
    }
}

bool GasNetwork::canCreateConnection(int startStationId, int endStationId) {
    if (stations.find(startStationId) == stations.end() ||
        stations.find(endStationId) == stations.end()) {
        return false;
    }

    if (startStationId == endStationId) {
        return false;
    }

    return true;
}

void GasNetwork::updateMatrices() {
    adjacencyMatrix.clear();
    weightMatrix.clear();

    // Инициализация матриц для всех КС
    for (const auto& stationPair : stations) {
        int stationId = stationPair.first;
        adjacencyMatrix[stationId] = std::map<int, int>();
        weightMatrix[stationId] = std::map<int, double>();

        for (const auto& otherStationPair : stations) {
            int otherId = otherStationPair.first;
            adjacencyMatrix[stationId][otherId] = 0;
            weightMatrix[stationId][otherId] = 0.0;
        }
    }

    // Заполнение матриц
    for (const auto& pipePair : pipes) {
        const Pipe& pipe = pipePair.second;

        if (pipe.getIsConnected() && pipes.find(pipe.getId()) != pipes.end()) {
            int startId = pipe.getStartStationId();
            int endId = pipe.getEndStationId();

            if (stations.find(startId) != stations.end() && stations.find(endId) != stations.end()) {
                // Матрица смежности: -1 для входа, 1 для выхода
                adjacencyMatrix[startId][endId] = 1;
                adjacencyMatrix[endId][startId] = -1;

                // Матрица весов
                weightMatrix[startId][endId] = pipe.getLength();
                weightMatrix[endId][startId] = pipe.getLength();
                }
            }   
    }
}

void GasNetwork::showAdjacencyMatrix() {
    if (stations.empty()) {
        std::cout << "There are no compressor stations to display the matrix.\n\n\n";
        return;
    }

    std::cout << "=== ADJACENCY MATRIX (Directed Graph) ===\n";
    std::cout << "    ";

    for (const auto& stationPair : stations) {
        std::cout << "CS" << stationPair.first << " ";
    }
    std::cout << "\n";

    for (const auto& rowPair : adjacencyMatrix) {
        int stationId = rowPair.first;
        std::cout << "CS" << stationId << " ";

        for (const auto& cellPair : rowPair.second) {
            int value = cellPair.second;

            if (value == 1) {
                std::cout << " -> ";
            }
            else if (value == -1) {
                std::cout << " <- ";
            }
            else {
                std::cout << " 0  ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "Legend: -> exit, <- input, 0 no connection\n\n";
}

void GasNetwork::showWeightMatrix() {
    if (stations.empty()) {
        std::cout << "There are no compressor stations to display the matrix.\n\n\n";
        return;
    }

    std::cout << "=== WEIGHT MATRIX (Pipe lengths in km) ===\n";
    std::cout << "    ";

    for (const auto& stationPair : stations) {
        std::cout << "CS" << stationPair.first << "   ";
    }
    std::cout << "\n";

    for (const auto& rowPair : weightMatrix) {
        int stationId = rowPair.first;
        std::cout << "CS" << stationId << " ";

        for (const auto& cellPair : rowPair.second) {
            double weight = cellPair.second;

            if (weight > 0) {
                std::cout << weight << "  ";
            }
            else {
                std::cout << " -   ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void GasNetwork::showConnectionsList() {
    std::cout << "=== CONNECTION LIST ===\n";

    bool hasConnections = false;
    for (const auto& pipePair : pipes) {
        const Pipe& pipe = pipePair.second;

        if (pipe.getIsConnected()) {
            hasConnections = true;
            std::cout << "CS" << pipe.getStartStationId() << " --Pipe#" << pipe.getId()
                << "[" << pipe.getDiameter() << "mm," << pipe.getLength() << "km]--> "
                << "CS" << pipe.getEndStationId() << "\n";
        }
    }

    if (!hasConnections) {
        std::cout << "No active connections.\n";
    }
    std::cout << "\n";
}

void GasNetwork::dfsTopologicalSort(int stationId, std::vector<bool>& visited, std::vector<int>& result) {
    visited[stationId] = true;

    for (const auto& neighborPair : adjacencyMatrix[stationId]) {
        int neighborId = neighborPair.first;
        int connectionType = neighborPair.second;

        if (connectionType == 1 && !visited[neighborId]) {
            dfsTopologicalSort(neighborId, visited, result);
        }
    }

    result.push_back(stationId);
}

bool GasNetwork::hasCycleDFS(int stationId, std::vector<bool>& visited, std::vector<bool>& recursionStack) {
    if (!visited[stationId]) {
        visited[stationId] = true;
        recursionStack[stationId] = true;

        for (const auto& neighborPair : adjacencyMatrix[stationId]) {
            int neighborId = neighborPair.first;
            int connectionType = neighborPair.second;

            if (connectionType == 1) {
                if (!visited[neighborId] && hasCycleDFS(neighborId, visited, recursionStack)) {
                    return true;
                }
                else if (recursionStack[neighborId]) {
                    return true;
                }
            }
        }
    }

    recursionStack[stationId] = false;
    return false;
}

std::vector<int> GasNetwork::topologicalSort() {
    std::vector<int> result;

    if (stations.empty()) {
        return result;
    }

    std::vector<int> verticesWithNoIncomingEdges;
    for (const auto& stationPair : stations) {
        int stationId = stationPair.first;
        bool hasIncomingEdges = false;

        for (const auto& otherStationPair : stations) {
            int otherId = otherStationPair.first;
            if (adjacencyMatrix[otherId][stationId] == 1) { 
                hasIncomingEdges = true;
                break;
            }
        }

        if (!hasIncomingEdges) {
            verticesWithNoIncomingEdges.push_back(stationId);
        }
    }

    if (verticesWithNoIncomingEdges.empty() && !stations.empty()) {
        std::vector<bool> visited(stations.size() + 1, false);
        std::vector<bool> recursionStack(stations.size() + 1, false);

        for (const auto& stationPair : stations) {
            int stationId = stationPair.first;
            if (hasCycleDFS(stationId, visited, recursionStack)) {
                std::cout << "Cycle detected! Topological sort is impossible.\n";
                return result;
            }
        }
    }

    std::vector<int> indegree(stations.size() + 1, 0);

    for (const auto& stationPair : stations) {
        int stationId = stationPair.first;
        for (const auto& otherStationPair : stations) {
            int otherId = otherStationPair.first;
            if (adjacencyMatrix[otherId][stationId] == 1) {
                indegree[stationId]++;
            }
        }
    }

    std::vector<int> queue;
    for (const auto& stationPair : stations) {
        int stationId = stationPair.first;
        if (indegree[stationId] == 0) {
            queue.push_back(stationId);
        }
    }

    while (!queue.empty()) {
        int current = queue.back();
        queue.pop_back();
        result.push_back(current);

        for (const auto& neighborPair : adjacencyMatrix[current]) {
            int neighborId = neighborPair.first;
            int connectionType = neighborPair.second;

            if (connectionType == 1) {
                indegree[neighborId]--;
                if (indegree[neighborId] == 0) {
                    queue.push_back(neighborId);
                }
            }
        }
    }

    if (result.size() != stations.size()) {
        std::cout << "Cycle detected! Topological sort is impossible (not all vertices processed).\n";
        return std::vector<int>();
    }

    return result;
}

std::vector<int> GasNetwork::getVerticesWithNoIncomingEdges() {
    std::vector<int> result;

    for (const auto& stationPair : stations) {
        int stationId = stationPair.first;
        bool hasIncomingEdges = false;

        for (const auto& otherStationPair : stations) {
            int otherId = otherStationPair.first;
            if (adjacencyMatrix[otherId][stationId] == 1) {
                hasIncomingEdges = true;
                break;
            }
        }

        if (!hasIncomingEdges) {
            result.push_back(stationId);
        }
    }

    return result;
}

double GasNetwork::calculateMaxFlow(int sourceId, int sinkId) {
    if (stations.find(sourceId) == stations.end() ||
        stations.find(sinkId) == stations.end()) {
        return 0.0;
    }

    if (sourceId == sinkId) {
        return std::numeric_limits<double>::infinity();
    }

    std::map<int, std::vector<std::pair<int, double>>> graph;

    for (const auto& station : stations) {
        graph[station.first] = std::vector<std::pair<int, double>>();
    }

    for (const auto& pipePair : pipes) {
        const Pipe& pipe = pipePair.second;
        if (pipe.getIsConnected()) {
            int from = pipe.getStartStationId();
            int to = pipe.getEndStationId();
            double capacity = pipe.getCapacity();

            graph[from].push_back({ to, capacity });
        }
    }

    double maxFlow = 0.0;

    while (true) {
        std::queue<int> q;
        std::map<int, int> parent;
        std::map<int, double> capacityTo;

        for (const auto& station : stations) {
            parent[station.first] = -1;
        }

        q.push(sourceId);
        parent[sourceId] = sourceId;
        capacityTo[sourceId] = std::numeric_limits<double>::infinity();

        bool foundPath = false;

        while (!q.empty() && !foundPath) {
            int current = q.front();
            q.pop();

            for (const auto& edge : graph[current]) {
                int neighbor = edge.first;
                double capacity = edge.second;

                if (parent[neighbor] == -1 && capacity > 1e-9) {
                    parent[neighbor] = current;
                    capacityTo[neighbor] = std::min(capacityTo[current], capacity);

                    if (neighbor == sinkId) {
                        foundPath = true;
                        break;
                    }

                    q.push(neighbor);
                }
            }
        }

        if (!foundPath) {
            break;
        }

        double pathFlow = capacityTo[sinkId];
        maxFlow += pathFlow;

        int current = sinkId;
        while (current != sourceId) {
            int prev = parent[current];

            for (auto& edge : graph[prev]) {
                if (edge.first == current) {
                    edge.second -= pathFlow;
                    break;
                }
            }

            bool hasReverseEdge = false;
            for (auto& edge : graph[current]) {
                if (edge.first == prev) {
                    edge.second += pathFlow;
                    hasReverseEdge = true;
                    break;
                }
            }

            if (!hasReverseEdge) {
                graph[current].push_back({ prev, pathFlow });
            }

            current = prev;
        }
    }

    return maxFlow;
}

std::pair<double, std::vector<int>> GasNetwork::calculateShortestPath(int startId, int endId) {
    std::map<int, double> dist;
    std::map<int, int> prev;
    std::set<std::pair<double, int>> queue;

    for (const auto& station : stations) {
        dist[station.first] = std::numeric_limits<double>::infinity();
        prev[station.first] = -1;
    }

    dist[startId] = 0;
    queue.insert({ 0, startId });

    while (!queue.empty()) {
        auto it = queue.begin();
        double currentDist = it->first;
        int u = it->second;
        queue.erase(it);

        if (u == endId) break;

        for (const auto& pipePair : pipes) {
            const Pipe& pipe = pipePair.second;
            if (pipe.getIsConnected() && pipe.getStartStationId() == u) {
                int v = pipe.getEndStationId();
                double weight = pipe.getWeightForPath();
                double newDist = currentDist + weight;

                if (newDist < dist[v]) {
                    auto oldPair = queue.find({ dist[v], v });
                    if (oldPair != queue.end()) {
                        queue.erase(oldPair);
                    }
                    dist[v] = newDist;
                    prev[v] = u;
                    queue.insert({ newDist, v });
                }
            }
        }
    }

    std::vector<int> path;
    if (dist[endId] < std::numeric_limits<double>::infinity()) {
        for (int at = endId; at != -1; at = prev[at]) {
            path.push_back(at);
        }
        std::reverse(path.begin(), path.end());
    }

    return std::make_pair(dist[endId], path);
}

void GasNetwork::calculateAndShowMaxFlow() {
    std::cout << "\n=== CALCULATE MAX FLOW ===\n";

    if (stations.size() < 2) {
        std::cout << "Need at least 2 CS!\n\n";
        return;
    }

    std::cout << "Available CS:\n";
    for (const auto& station : stations) {
        std::cout << "CS" << station.first << " - " << station.second.getName() << "\n";
    }

    int sourceId, sinkId;
    if (!isValidInput(sourceId, "Enter ID of CS start: ")) {
        std::cout << "Invalid ID!\n\n";
        return;
    }

    if (!isValidInput(sinkId, "Enter ID of CS end: ")) {
        std::cout << "Invalid ID!\n\n";
        return;
    }

    double maxFlow = calculateMaxFlow(sourceId, sinkId);

    std::cout << "\nResults:\n";
    std::cout << "Max flow from CS" << sourceId
        << " to CS" << sinkId << ": "
        << maxFlow << " mln m^3/day\n\n";

    if (maxFlow == 0) {
        std::cout << "Possible problems:\n";
        std::cout << "1. No connections between CS\n";
        std::cout << "2. All pipes under renovation\n\n";
    }
}

void GasNetwork::calculateAndShowShortestPath() {
    std::cout << "\n=== Find a shortest path ===\n";

    if (stations.size() < 2) {
        std::cout << "You need to have min 2 CS!\n\n";
        return;
    }

    std::cout << "Avaible CS:\n";
    for (const auto& station : stations) {
        std::cout << "CS" << station.first << " - " << station.second.getName() << "\n";
    }

    int startId, endId;
    std::cout << "Enter ID of CS start: ";
    while (!isValidInput(startId, "Enter startId to edit: ")) {
        std::cout << "Invalid ID!\n\n";
    }
    std::cout << "Enter ID of CS end: ";
    while (!isValidInput(endId, "Enter endId to edit: ")) {
        std::cout << "Invalid ID!\n\n";
    }

    auto result = calculateShortestPath(startId, endId);
    double distance = result.first;
    std::vector<int> path = result.second;

    std::cout << "\nResults:\n";
    if (path.empty()) {
        std::cout << "Path not found!\n\n";
    }
    else {
        std::cout << "Shortest path from CS" << startId
            << " to CS" << endId << ":\n";
        std::cout << "Global lenght: " << distance << "km\n";
        std::cout << "path: ";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << "CS" << path[i];
            if (i < path.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << "\n\n";
    }
}