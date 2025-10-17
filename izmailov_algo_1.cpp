#include <iostream>
#include <map>
#include "pipe.h"
#include "CompressStation.h"
#include "logger.h"
#include "utils.h"
#include "file_operations.h"
#include "operations.h"
#include "globals.h"

void showMenu(std::map<int, Pipe>& pipes, std::map<int, CompressStation>& stations, Logger& logger) {
    int menu_choose;

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
            << "0. Exit\n";

        logger.logCommand("MENU_SHOWN");

        if (!isValidInput(menu_choose, "Enter your choice: ")) {
            std::cout << "Invalid input! Please enter a number.\n\n";
            logger.logUserInput("INVALID_INPUT");
            continue;
        }

        logger.logUserInput(std::to_string(menu_choose));

        switch (menu_choose) {
        case 1:
            logger.logCommand("ADD_PIPE_STARTED");
            addPipe(pipes, logger);
            break;

        case 2:
            logger.logCommand("ADD_CS_STARTED");
            addCompressStation(stations, logger);
            break;

        case 3:
            logger.logCommand("VIEW_ALL_OBJECTS");
            viewAllObjects(pipes, stations);
            break;

        case 4: {
            if (pipes.empty()) {
                std::cout << "No pipes available to edit.\n\n";
                logger.logCommand("EDIT_PIPE_NO_PIPES");
                break;
            }

            logger.logCommand("EDIT_PIPE_ID_INPUT");
            int pipeId;
            if (!isValidInput(pipeId, "Enter pipe ID to edit: ")) {
                std::cout << "Invalid ID!\n\n";
                logger.logUserInput("INVALID_PIPE_ID");
                break;
            }
            logger.logUserInput(std::to_string(pipeId));

            if (pipes.find(pipeId) == pipes.end()) {
                std::cout << "Pipe with ID " << pipeId << " not found.\n\n";
                logger.logCommand("PIPE_NOT_FOUND: ID=" + std::to_string(pipeId));
                break;
            }

            logger.logCommand("EDIT_PIPE_STATUS_CHOICE");
            std::cout << "Current status: " << (pipes[pipeId].getStatus() ? "UNDER RENOVATION" : "WORKING") << "\n";
            std::cout << "Select new repair status:\n";
            std::cout << "1. Put UNDER RENOVATION (repair ON)\n";
            std::cout << "2. Set to WORKING (repair OFF)\n";
            std::cout << "3. TOGGLE status\n";

            int statusChoice;
            if (!isValidInput(statusChoice, "Enter your choice: ")) {
                std::cout << "Invalid choice!\n\n";
                logger.logUserInput("INVALID_STATUS_CHOICE");
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
            case 3:
                newStatus = !pipes[pipeId].getStatus();
                statusText = newStatus ? "UNDER RENOVATION" : "WORKING";
                break;
            default:
                std::cout << "Invalid choice!\n\n";
                logger.logUserInput("INVALID_STATUS_VALUE");
                break;
            }

            if (statusChoice >= 1 && statusChoice <= 3) {
                pipes[pipeId].setStatus(newStatus);
                std::cout << "Pipe status updated to: " << statusText << "\n\n";
                logger.logCommand("PIPE_STATUS_UPDATED: ID=" + std::to_string(pipeId) +
                    ", status=" + statusText);
            }
            break;
        }

        case 5: {
            if (stations.empty()) {
                std::cout << "No CS available to edit.\n\n";
                logger.logCommand("EDIT_CS_NO_STATIONS");
                break;
            }

            logger.logCommand("EDIT_CS_ID_INPUT");
            int stationId;
            if (!isValidInput(stationId, "Enter CS ID to edit: ")) {
                std::cout << "Invalid ID!\n\n";
                logger.logUserInput("INVALID_CS_ID");
                break;
            }
            logger.logUserInput(std::to_string(stationId));

            if (stations.find(stationId) == stations.end()) {
                std::cout << "CS with ID " << stationId << " not found.\n\n";
                logger.logCommand("CS_NOT_FOUND: ID=" + std::to_string(stationId));
                break;
            }

            logger.logCommand("EDIT_CS_WORKSHOPS_INPUT");
            int newWorkshopsInWork;
            if (!isValidInput(newWorkshopsInWork, "Enter new number of workshops in work: ")) {
                std::cout << "Invalid number!\n\n";
                logger.logUserInput("INVALID_WORKSHOPS_COUNT");
                break;
            }
            logger.logUserInput(std::to_string(newWorkshopsInWork));

            if (stations[stationId].changeNumberOfWorkshopsInWork(newWorkshopsInWork)) {
                std::cout << "CS updated successfully!\n\n";
                logger.logCommand("CS_WORKSHOPS_UPDATED: ID=" + std::to_string(stationId) +
                    ", count=" + std::to_string(newWorkshopsInWork));
            }
            else {
                std::cout << "Invalid number of workshops in work!\n\n";
                logger.logUserInput("INVALID_WORKSHOPS_VALUE");
            }
            break;
        }

        case 6: {
            logger.logCommand("SEARCH_PIPES_STARTED");
            std::cout << "Search pipes by:\n"
                << "1. ID\n"
                << "2. Name\n"
                << "3. Repair status\n";

            int searchChoice;
            if (!isValidInput(searchChoice, "Enter your choice: ")) {
                std::cout << "Invalid choice!\n\n";
                logger.logUserInput("INVALID_SEARCH_CHOICE");
                break;
            }
            logger.logUserInput(std::to_string(searchChoice));

            std::vector<int> foundPipes;

            if (searchChoice == 1) {
                logger.logCommand("SEARCH_PIPES_BY_ID");
                int searchId;
                if (!isValidInput(searchId, "Enter pipe ID to search: ")) {
                    std::cout << "Invalid ID!\n\n";
                    logger.logUserInput("INVALID_SEARCH_ID");
                    break;
                }
                logger.logUserInput(std::to_string(searchId));
                foundPipes = findPipesById(pipes, searchId);
                logger.logCommand("PIPES_SEARCH_COMPLETED: by_id=" + std::to_string(searchId));

            }
            else if (searchChoice == 2) {
                logger.logCommand("SEARCH_PIPES_BY_NAME");
                std::string name;
                if (!isValidInput(name, "Enter name to search: ")) {
                    std::cout << "Invalid name!\n\n";
                    logger.logUserInput("INVALID_SEARCH_NAME");
                    break;
                }
                logger.logUserInput(name);
                foundPipes = findPipesByName(pipes, name);
                logger.logCommand("PIPES_SEARCH_COMPLETED: by_name=" + name);

            }
            else if (searchChoice == 3) {
                logger.logCommand("SEARCH_PIPES_BY_STATUS");
                std::cout << "Search by status:\n"
                    << "1. Under renovation\n"
                    << "2. Working\n";

                int statusChoice;
                if (!isValidInput(statusChoice, "Enter your choice: ")) {
                    std::cout << "Invalid choice!\n\n";
                    logger.logUserInput("INVALID_STATUS_SEARCH_CHOICE");
                    break;
                }
                logger.logUserInput(std::to_string(statusChoice));

                foundPipes = findPipesByStatus(pipes, statusChoice == 1);
                logger.logCommand("PIPES_SEARCH_COMPLETED: by_status=" + std::to_string(statusChoice));

            }
            else {
                std::cout << "Invalid choice!\n\n";
                logger.logUserInput("INVALID_SEARCH_OPTION");
                break;
            }

            if (foundPipes.empty()) {
                std::cout << "No pipes found.\n\n";
                logger.logCommand("SEARCH_NO_RESULTS");
            }
            else {
                std::cout << "Found " << foundPipes.size() << " pipes:\n";
                for (int id : foundPipes) {
                    pipes[id].displayInfo();
                }
                logger.logCommand("SEARCH_RESULTS: count=" + std::to_string(foundPipes.size()));
            }
            break;
        }

        case 7: {
            logger.logCommand("SEARCH_CS_STARTED");
            std::cout << "Search CS by:\n"
                << "1. ID\n"
                << "2. Name\n"
                << "3. Unused percentage\n";

            int searchChoice;
            if (!isValidInput(searchChoice, "Enter your choice: ")) {
                std::cout << "Invalid choice!\n\n";
                logger.logUserInput("INVALID_CS_SEARCH_CHOICE");
                break;
            }
            logger.logUserInput(std::to_string(searchChoice));

            std::vector<int> foundStations;

            if (searchChoice == 1) {
                logger.logCommand("SEARCH_CS_BY_ID");
                int searchId;
                if (!isValidInput(searchId, "Enter CS ID to search: ")) {
                    std::cout << "Invalid ID!\n\n";
                    logger.logUserInput("INVALID_CS_SEARCH_ID");
                    break;
                }
                logger.logUserInput(std::to_string(searchId));
                foundStations = findStationsById(stations, searchId);
                logger.logCommand("CS_SEARCH_COMPLETED: by_id=" + std::to_string(searchId));

            }
            else if (searchChoice == 2) {
                logger.logCommand("SEARCH_CS_BY_NAME");
                std::string name;
                if (!isValidInput(name, "Enter name to search: ")) {
                    std::cout << "Invalid name!\n\n";
                    logger.logUserInput("INVALID_CS_SEARCH_NAME");
                    break;
                }
                logger.logUserInput(name);
                foundStations = findStationsByName(stations, name);
                logger.logCommand("CS_SEARCH_COMPLETED: by_name=" + name);

            }
            else if (searchChoice == 3) {
                logger.logCommand("SEARCH_CS_BY_PERCENTAGE");
                double minPercent;
                if (!isValidInput(minPercent, "Enter minimum unused percentage: ")) {
                    std::cout << "Invalid percentage!\n\n";
                    logger.logUserInput("INVALID_PERCENTAGE");
                    break;
                }
                logger.logUserInput(std::to_string(minPercent));

                foundStations = findStationsByUnusedPercentage(stations, minPercent);
                logger.logCommand("CS_SEARCH_COMPLETED: by_percentage=" + std::to_string(minPercent));

            }
            else {
                std::cout << "Invalid choice!\n\n";
                logger.logUserInput("INVALID_CS_SEARCH_OPTION");
                break;
            }

            if (foundStations.empty()) {
                std::cout << "No CS found.\n\n";
                logger.logCommand("CS_SEARCH_NO_RESULTS");
            }
            else {
                std::cout << "Found " << foundStations.size() << " CS:\n";
                for (int id : foundStations) {
                    stations[id].displayInfo();
                }
                logger.logCommand("CS_SEARCH_RESULTS: count=" + std::to_string(foundStations.size()));
            }
            break;
        }

        case 8: {
            logger.logCommand("BATCH_EDIT_PIPES_STARTED");
            batchEditPipes(pipes, logger);
            break;
        }

        case 9: {
            logger.logCommand("SAVE_DATA_STARTED");
            std::string filename;
            if (!isValidInput(filename, "Enter filename to save: ")) {
                std::cout << "Invalid filename!\n\n";
                logger.logUserInput("INVALID_FILENAME");
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
                logger.logCommand("SAVE_COMPLETED: filename=" + filename);
            }
            else {
                std::cout << "Save cancelled.\n\n";
                logger.logCommand("SAVE_CANCELLED");
            }
            break;
        }

        case 10: {
            logger.logCommand("LOAD_DATA_STARTED");
            std::string filename;
            if (!isValidInput(filename, "Enter filename to load: ")) {
                std::cout << "Invalid filename!\n\n";
                logger.logUserInput("INVALID_FILENAME");
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
                logger.logCommand("LOAD_COMPLETED: filename=" + filename);
            }
            else {
                std::cout << "Load cancelled.\n\n";
                logger.logCommand("LOAD_CANCELLED");
            }
            break;
        }

        case 11:
            logger.logCommand("EDIT_PIPE_BY_ID_STARTED");
            editPipe(pipes, logger);
            break;

        case 12:
            logger.logCommand("EDIT_CS_BY_ID_STARTED");
            editCompressStation(stations, logger);
            break;

        case 0:
            logger.logCommand("EXIT_SELECTED");
            std::cout << "Goodbye!\n";
            return;

        default:
            std::cout << "Invalid choice! Please enter a number between 0 and 12.\n\n";
            logger.logUserInput("INVALID_MENU_CHOICE: " + std::to_string(menu_choose));
            break;
        }
    }
}

int main() {
    std::map<int, Pipe> pipes;
    std::map<int, CompressStation> stations;
    Logger logger("user_commands_log.txt");

    std::cout << "Welcome to the application 'Gas and Oil Pipeline Transportation System'\n";
    logger.logCommand("APPLICATION_STARTED");

    showMenu(pipes, stations, logger);

    return 0;
}