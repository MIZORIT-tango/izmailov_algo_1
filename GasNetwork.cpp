#include "GasNetwork.h"
#include "utils.h"
#include <iostream>
#include <algorithm>
#include <stack>

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

    // Рекурсивно посещаем все соседние КС (выходы)
    for (const auto& neighborPair : adjacencyMatrix[stationId]) {
        int neighborId = neighborPair.first;
        int connectionType = neighborPair.second;

        // Если есть выход к этой КС и она не посещена
        if (connectionType == 1 && !visited[neighborId]) {
            dfsTopologicalSort(neighborId, visited, result);
        }
    }

    // Добавляем текущую КС в результат (после всех её зависимостей)
    result.push_back(stationId);
}

bool GasNetwork::hasCycleDFS(int stationId, std::vector<bool>& visited, std::vector<bool>& recursionStack) {
    if (!visited[stationId]) {
        visited[stationId] = true;
        recursionStack[stationId] = true;

        // Проверяем всех соседей
        for (const auto& neighborPair : adjacencyMatrix[stationId]) {
            int neighborId = neighborPair.first;
            int connectionType = neighborPair.second;

            // Если есть выход к соседу
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

    // Проверка на циклы
    std::vector<bool> visited(stations.size() + 1, false);
    std::vector<bool> recursionStack(stations.size() + 1, false);

    for (const auto& stationPair : stations) {
        int stationId = stationPair.first;
        if (!visited[stationId] && hasCycleDFS(stationId, visited, recursionStack)) {
            std::cout << "Обнаружен цикл в графе! Топологическая сортировка невозможна.\n";
            return result;
        }
    }

    // Сброс visited для основной сортировки
    std::fill(visited.begin(), visited.end(), false);
    result.clear();

    // DFS для топологической сортировки
    for (const auto& stationPair : stations) {
        int stationId = stationPair.first;
        if (!visited[stationId]) {
            dfsTopologicalSort(stationId, visited, result);
        }
    }

    // Реверсируем результат (так как добавляли в конец)
    std::reverse(result.begin(), result.end());

    return result;
}