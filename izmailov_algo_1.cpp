#include <iostream>
#include <map>
#include "pipe.h"
#include "CompressStation.h"
#include "logger.h"
#include "utils.h"
#include "file_operations.h"
#include "operations.h"
#include "globals.h"
#include "GasNetwork.h"

void showMenu(std::map<int, Pipe>& pipes, std::map<int, CompressStation>& stations, Logger& logger) {
    int menu_choose;
    GasNetwork gasNetwork(pipes, stations);

    while (true) {
        std::cout << "Select an action:\n"
            << "1. Add pipe\n"
            << "2. Add CS\n"
            << "3. View all objects\n"
            << "4. Edit pipe repair status\n"
            << "5. Edit CS workshops in work\n"
            << "6. Search pipes\n"
            << "7. Search CS\n"
            << "8. Batch edit pipes\n"
            << "9. Save\n"
            << "10. Load\n"
            << "11. Edit pipe by ID (all fields)\n"
            << "12. Edit CS by ID (all fields)\n"
            << "13. Create connection\n"
            << "14. Show adjacency matrix\n"
            << "15. Show weight matrix\n"
            << "16. Show connections list\n"
            << "17. Topological sort\n"
            << "18. Batch delete CS\n"    
            << "19. Destroy connection\n" 
            << "0. Exit\n";

        if (!isValidInput(menu_choose, "Enter your choice: ")) {
            std::cout << "Invalid input! Please enter a number.\n\n";
            continue;
        }

        logger.logUserInput(std::to_string(menu_choose));

        switch (menu_choose) {
        case 1:
            addPipe(pipes, logger);
            break;

        case 2:
            addCompressStation(stations, logger);
            break;

        case 3:
            viewAllObjects(pipes, stations);
            break;

        case 4: {
            if (pipes.empty()) {
                std::cout << "No pipes available to edit.\n\n";
                break;
            }

            int pipeId;
            if (!isValidInput(pipeId, "Enter pipe ID to edit: ")) {
                std::cout << "Invalid ID!\n\n";
                break;
            }
            logger.logUserInput(std::to_string(pipeId));

            if (pipes.find(pipeId) == pipes.end()) {
                std::cout << "Pipe with ID " << pipeId << " not found.\n\n";
                break;
            }

            std::cout << "Current status: " << (pipes[pipeId].getStatus() ? "UNDER RENOVATION" : "WORKING") << "\n";
            std::cout << "Select new repair status:\n";
            std::cout << "1. Put UNDER RENOVATION (repair ON)\n";
            std::cout << "2. Set to WORKING (repair OFF)\n";

            int statusChoice;
            if (!isValidInput(statusChoice, "Enter your choice: ")) {
                std::cout << "Invalid choice!\n\n";
                break;
            }
            logger.logUserInput(std::to_string(statusChoice));

            bool newStatus;
            std::string statusText;

            switch (statusChoice) {
            case 1:
                newStatus = true;
                statusText = "UNDER RENOVATION";
                break;
            case 2:
                newStatus = false;
                statusText = "WORKING";
                break;
            default:
                std::cout << "Invalid choice!\n\n";
                break;
            }

            if (statusChoice >= 1 && statusChoice <= 2) {
                pipes[pipeId].setStatus(newStatus);
                std::cout << "Pipe status updated to: " << statusText << "\n\n";
            }
            break;
        }

        case 5: {
            if (stations.empty()) {
                std::cout << "No CS available to edit.\n\n";
                break;
            }

            int stationId;
            if (!isValidInput(stationId, "Enter CS ID to edit: ")) {
                std::cout << "Invalid ID!\n\n";
                break;
            }
            logger.logUserInput(std::to_string(stationId));

            if (stations.find(stationId) == stations.end()) {
                std::cout << "CS with ID " << stationId << " not found.\n\n";
                break;
            }

            int newWorkshopsInWork;
            if (!isValidInput(newWorkshopsInWork, "Enter new number of workshops in work: ")) {
                std::cout << "Invalid number!\n\n";
                break;
            }
            logger.logUserInput(std::to_string(newWorkshopsInWork));

            if (stations[stationId].changeNumberOfWorkshopsInWork(newWorkshopsInWork)) {
                std::cout << "CS updated successfully!\n\n";
            }
            else {
                std::cout << "Invalid number of workshops in work!\n\n";
            }
            break;
        }

        case 6: {
            std::cout << "Search pipes by:\n"
                << "1. ID\n"
                << "2. Name\n"
                << "3. Repair status\n";

            int searchChoice;
            if (!isValidInput(searchChoice, "Enter your choice: ")) {
                std::cout << "Invalid choice!\n\n";
                break;
            }
            logger.logUserInput(std::to_string(searchChoice));

            std::map<int, Pipe> foundPipes;

            if (searchChoice == 1) {
                int searchId;
                if (!isValidInput(searchId, "Enter pipe ID to search: ")) {
                    std::cout << "Invalid ID!\n\n";
                    break;
                }
                logger.logUserInput(std::to_string(searchId));
                foundPipes = findPipesById(pipes, searchId);
            }
            else if (searchChoice == 2) {
                std::string name;
                if (!isValidInput(name, "Enter name to search: ")) {
                    std::cout << "Invalid name!\n\n";
                    break;
                }
                logger.logUserInput(name);
                foundPipes = findPipesByName(pipes, name);
            }
            else if (searchChoice == 3) {
                std::cout << "Search by status:\n"
                    << "1. Under renovation\n"
                    << "2. Working\n";

                int statusChoice;
                if (!isValidInput(statusChoice, "Enter your choice: ")) {
                    std::cout << "Invalid choice!\n\n";
                    break;
                }
                logger.logUserInput(std::to_string(statusChoice));

                foundPipes = findPipesByStatus(pipes, statusChoice == 1);
            }
            else {
                std::cout << "Invalid choice!\n\n";
                break;
            }

            if (foundPipes.empty()) {
                std::cout << "No pipes found.\n\n";
            }
            else {
                std::cout << "\nFound " << foundPipes.size() << " pipes:\n\n";
                for (const auto& pair : foundPipes) {
                    std::cout << pair.second;
                }
            }
            break;
        }

        case 7: {
            std::cout << "Search CS by:\n"
                << "1. ID\n"
                << "2. Name\n"
                << "3. Unused percentage\n";

            int searchChoice;
            if (!isValidInput(searchChoice, "Enter your choice: ")) {
                std::cout << "Invalid choice!\n\n";
                break;
            }
            logger.logUserInput(std::to_string(searchChoice));

            std::map<int, CompressStation> foundStations;

            if (searchChoice == 1) {
                int searchId;
                if (!isValidInput(searchId, "Enter CS ID to search: ")) {
                    std::cout << "Invalid ID!\n\n";
                    break;
                }
                logger.logUserInput(std::to_string(searchId));
                foundStations = findStationsById(stations, searchId);
            }
            else if (searchChoice == 2) {
                std::string name;
                if (!isValidInput(name, "Enter name to search: ")) {
                    std::cout << "Invalid name!\n\n";
                    break;
                }
                logger.logUserInput(name);
                foundStations = findStationsByName(stations, name);
            }
            else if (searchChoice == 3) {
                double minPercent;
                if (!isValidInput(minPercent, "Enter minimum unused percentage: ")) {
                    std::cout << "Invalid percentage!\n\n";
                    break;
                }
                logger.logUserInput(std::to_string(minPercent));
                foundStations = findStationsByUnusedPercentage(stations, minPercent);
            }
            else {
                std::cout << "Invalid choice!\n\n";
                break;
            }

            if (foundStations.empty()) {
                std::cout << "No CS found.\n\n";
            }
            else {
                std::cout << "\nFound " << foundStations.size() << " CS:\n\n";
                for (const auto& pair : foundStations) {
                    std::cout << pair.second;
                }
            }
            break;
        }

        case 8:
            batchEditPipes(pipes, logger);
            gasNetwork.updateMatrices();
            break;

        case 9: {
            std::string filename;
            if (!isValidInput(filename, "Enter filename to save: ")) {
                std::cout << "Invalid filename!\n\n";
                break;
            }
            logger.logUserInput(filename);

            std::cout << "Are you sure? The file will be permanently replaced.\n";
            std::cout << "Choice: Y/N (Yes/No)\n";

            char choice;
            std::cin >> choice;
            clearInputBuffer();
            logger.logUserInput(std::string(1, choice));

            if (tolower(choice) == 'y') {
                saveData(filename, pipes, stations, logger);
            }
            else {
                std::cout << "Save cancelled.\n\n";
            }
            break;
        }

        case 10: {
            std::string filename;
            if (!isValidInput(filename, "Enter filename to load: ")) {
                std::cout << "Invalid filename!\n\n";
                break;
            }
            logger.logUserInput(filename);

            std::cout << "Are you sure? The current data will be replaced.\n";
            std::cout << "Choice: Y/N (Yes/No)\n";

            char choice;
            std::cin >> choice;
            clearInputBuffer();
            logger.logUserInput(std::string(1, choice));

            if (tolower(choice) == 'y') {
                loadData(filename, pipes, stations, logger);
                gasNetwork.updateMatrices();
                std::cout << "Data loaded successfully from " << filename << "\n";
                std::cout << "Matrices updated.\n\n";
            }
            else {
                std::cout << "Load cancelled.\n\n";
            }
            break;
        }

        case 11:
            editPipe(pipes, logger);
            break;

        case 12:
            editCompressStation(stations, logger);
            break;

        case 13: {
            if (stations.size() < 2) {
                std::cout << "You need at least 2 CS to create a connection.!\n\n";
                break;
            }

            int startStationId, endStationId, diameter;

            if (!isValidInput(startStationId, "Enter START station ID: ")) {
                std::cout << "Invalid ID!\n\n";
                break;
            }
            logger.logUserInput(std::to_string(startStationId));

            if (!isValidInput(endStationId, "Enter END station ID: ")) {
                std::cout << "Invalid ID!\n\n";
                break;
            }
            logger.logUserInput(std::to_string(endStationId));

            if (!isValidInput(diameter, "Enter pipe diameter: ")) {
                std::cout << "Invalid diameter!\n\n";
                break;
            }
            logger.logUserInput(std::to_string(diameter));

            gasNetwork.createConnection(startStationId, endStationId, diameter);
            break;
        }

        case 14:
            gasNetwork.showAdjacencyMatrix();
            break;

        case 15:
            gasNetwork.showWeightMatrix();
            break;

        case 16:
            gasNetwork.showConnectionsList();
            break;

        case 17: {
            std::vector<int> sortedStations = gasNetwork.topologicalSort();

            if (sortedStations.empty()) {
                std::cout << "It is impossible to perform topological sorting.\n";
                std::cout << "Possible causes: empty graph, cycles in the graph.\n\n";
            }
            else {
                std::cout << "=== TOPOLOGICAL SORTING ===\n";
                std::cout << "CS processing procedure: ";
                for (size_t i = 0; i < sortedStations.size(); ++i) {
                    std::cout << "CS" << sortedStations[i];
                    if (i < sortedStations.size() - 1) {
                        std::cout << " -> ";
                    }
                }
                std::cout << "\n\n";
            }
            break;
        }

        case 18:
            batchDeleteStations(stations, pipes, logger);
            gasNetwork.updateMatrices();
            break;

        case 19:
            destroyConnection(pipes, logger);
            gasNetwork.updateMatrices();
            break;

        case 0:
            std::cout << "Goodbye!\n";
            return;

        default:
            std::cout << "Invalid choice! Please enter a number between 0 and 17.\n\n";
            break;
        }
    }
}

int main() {
    std::map<int, Pipe> pipes;
    std::map<int, CompressStation> stations;
    Logger logger("user_commands_log.txt");

    std::cout << "Welcome to the application 'Gas and Oil Pipeline Transportation System'\n";

    showMenu(pipes, stations, logger);

    return 0;
}