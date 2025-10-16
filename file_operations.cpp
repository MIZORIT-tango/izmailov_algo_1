#include "file_operations.h"
#include "utils.h"
#include <fstream>
#include <iostream>


void saveData(const std::string& filename, const std::map<int, Pipe>& pipes,
    const std::map<int, CompressStation>& stations, Logger& logger) {
    std::ofstream file(filename);

    if (file.is_open()) {
        // Сохраняем трубы с тегами
        for (const auto& pair : pipes) {
            file << PIPE_START_TAG << "\n";
            file << pair.second;
            file << PIPE_END_TAG << "\n\n";
        }

        // Сохраняем КС с тегами
        for (const auto& pair : stations) {
            file << STATION_START_TAG << "\n";
            file << pair.second;
            file << STATION_END_TAG << "\n\n";
        }

        file.close();
        std::cout << "Data saved to " << filename << "\n\n";
        logger.logCommand("DATA_SAVED: pipes=" + std::to_string(pipes.size()) +
            ", stations=" + std::to_string(stations.size()));
    }
    else {
        std::cout << "Error saving data to file.\n\n";
        logger.logCommand("SAVE_ERROR: filename=" + filename);
    }
}

void loadData(const std::string& filename, std::map<int, Pipe>& pipes,
    std::map<int, CompressStation>& stations, Logger& logger) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Error opening file for reading!\n\n";
        logger.logCommand("LOAD_ERROR: file_not_found=" + filename);
        return;
    }

    pipes.clear();
    stations.clear();

    std::string line;

    try {
        while (std::getline(file, line)) {
            line = trim(line);

            if (line.empty()) continue;

            if (line == PIPE_START_TAG) {
                // Читаем данные трубы
                int id;
                std::string name;
                float length;
                int diameter;
                bool status;

                file >> id;
                file.ignore();
                std::getline(file, name);
                file >> length >> diameter >> status;

                Pipe pipe(id, name, length, diameter);
                if (status) {
                    pipe.setStatus(true);
                }

                pipes[pipe.getId()] = pipe;

                std::getline(file, line);
                std::getline(file, line);
            }
            else if (line == STATION_START_TAG) {
                // Читаем данные КС
                int id;
                std::string name;
                int workshops, workshops_in_work;
                std::string class_cs;

                file >> id;
                file.ignore();
                std::getline(file, name);
                file >> workshops >> workshops_in_work;
                file.ignore();
                std::getline(file, class_cs);

                CompressStation station(id, name, workshops, workshops_in_work, class_cs);
                stations[station.getId()] = station;

                std::getline(file, line);
            }
        }

        std::cout << "Data loaded successfully from " << filename << "\n\n";
        logger.logCommand("DATA_LOADED: pipes=" + std::to_string(pipes.size()) +
            ", stations=" + std::to_string(stations.size()));

    }
    catch (const std::exception& e) {
        std::cout << "Error reading file: " << e.what() << "\n" << std::endl;
        logger.logCommand("LOAD_ERROR: " + std::string(e.what()));
        pipes.clear();
        stations.clear();
    }

    file.close();
}