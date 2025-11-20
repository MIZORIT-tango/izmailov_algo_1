#include "operations.h"
#include <algorithm>
#include <iostream>

void addPipe(std::map<int, Pipe>& pipes, Logger& logger) {
    std::string name;
    float length;
    int diameter;

    if (!isValidInput(name, "Enter name of pipe:\n")) {
        std::cout << "Invalid name!\n\n";
        return;
    }
    logger.logUserInput(name);

    if (!isValidInput(length, "Enter length of pipe:\n") || length <= 0) {
        std::cout << "Invalid length!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(length));

    if (!isValidInput(diameter, "Enter diameter of pipe:\n") || diameter <= 0) {
        std::cout << "Invalid diameter!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(diameter));

    int nextId = getNextPipeId();
    Pipe newPipe(nextId, name, length, diameter);
    pipes[newPipe.getId()] = newPipe;
    std::cout << "Pipe created successfully! ID: " << newPipe.getId() << "\n\n";
}

void addCompressStation(std::map<int, CompressStation>& stations, Logger& logger) {
    std::string name, class_cs;
    int workshops, workshops_in_work;

    if (!isValidInput(name, "Enter name of CS:\n")) {
        std::cout << "Invalid name!\n\n";
        return;
    }
    logger.logUserInput(name);

    if (!isValidInput(workshops, "Enter number of workshops:\n") || workshops <= 0) {
        std::cout << "Invalid number!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(workshops));

    if (!isValidInput(workshops_in_work, "Enter number of workshops in work:\n")) {
        std::cout << "Invalid number!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(workshops_in_work));

    if (workshops_in_work > workshops || workshops_in_work < 0) {
        std::cout << "Invalid number of workshops in work!\n\n";
        return;
    }

    if (!isValidInput(class_cs, "Enter class of CS:\n")) {
        std::cout << "Invalid class!\n\n";
        return;
    }
    logger.logUserInput(class_cs);

    int nextId = getNextStationId();
    CompressStation newCS(nextId, name, workshops, workshops_in_work, class_cs);
    stations[newCS.getId()] = newCS;
    std::cout << "CS created successfully! ID: " << newCS.getId() << "\n\n";
}

void viewAllObjects(const std::map<int, Pipe>& pipes, const std::map<int, CompressStation>& stations) {
    std::cout << "=== PIPES ===\n";
    if (pipes.empty()) {
        std::cout << "No pipes available.\n";
    }
    else {
        for (const auto& pair : pipes) {
            std::cout << pair.second;
        }
    }

    std::cout << "=== COMPRESS STATIONS ===\n";
    if (stations.empty()) {
        std::cout << "No compress stations available.\n";
    }
    else {
        for (const auto& pair : stations) {
            std::cout << pair.second;
        }
    }
    std::cout << std::endl;
}

std::map<int, Pipe> findPipesByName(const std::map<int, Pipe>& pipes, const std::string& name) {
    std::map<int, Pipe> result;
    for (const auto& pair : pipes) {
        if (pair.second.getName().find(name) != std::string::npos) {
            result[pair.first] = pair.second;
        }
    }
    return result;
}

std::map<int, Pipe> findPipesByStatus(const std::map<int, Pipe>& pipes, bool status) {
    std::map<int, Pipe> result;
    for (const auto& pair : pipes) {
        if (pair.second.getStatus() == status) {
            result[pair.first] = pair.second;
        }
    }
    return result;
}

std::map<int, Pipe> findPipesById(const std::map<int, Pipe>& pipes, int id) {
    std::map<int, Pipe> result;
    if (pipes.find(id) != pipes.end()) {
        result[id] = pipes.at(id);
    }
    return result;
}

std::map<int, CompressStation> findStationsByName(const std::map<int, CompressStation>& stations, const std::string& name) {
    std::map<int, CompressStation> result;
    for (const auto& pair : stations) {
        if (pair.second.getName().find(name) != std::string::npos) {
            result[pair.first] = pair.second;
        }
    }
    return result;
}

std::map<int, CompressStation> findStationsByUnusedPercentage(const std::map<int, CompressStation>& stations, double minPercent, double maxPercent) {
    std::map<int, CompressStation> result;
    for (const auto& pair : stations) {
        double percent = pair.second.getUnusedPercentage();
        if (percent >= minPercent && percent <= maxPercent) {
            result[pair.first] = pair.second;
        }
    }
    return result;
}

std::map<int, CompressStation> findStationsById(const std::map<int, CompressStation>& stations, int id) {
    std::map<int, CompressStation> result;
    if (stations.find(id) != stations.end()) {
        result[id] = stations.at(id);
    }
    return result;
}

void editPipe(std::map<int, Pipe>& pipes, Logger& logger) {
    if (pipes.empty()) {
        std::cout << "No pipes available to edit.\n\n";
        return;
    }

    int pipeId;
    if (!isValidInput(pipeId, "Enter pipe ID to edit: ")) {
        std::cout << "Invalid ID!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(pipeId));

    if (pipes.find(pipeId) == pipes.end()) {
        std::cout << "Pipe with ID " << pipeId << " not found.\n\n";
        return;
    }

    Pipe& pipe = pipes[pipeId];

    std::cout << "\nCurrent pipe data:\n";
    std::cout << pipe;

    int fieldChoice;
    do {
        std::cout << "Select field to edit:\n";
        std::cout << "1. Name (current: " << pipe.getName() << ")\n";
        std::cout << "2. Length (current: " << pipe.getLength() << ")\n";
        std::cout << "3. Diameter (current: " << pipe.getDiameter() << ")\n";
        std::cout << "4. Repair status (current: " << (pipe.getStatus() ? "Under renovation" : "Working") << ")\n";
        std::cout << "0. Finish editing\n";

        if (!isValidInput(fieldChoice, "Enter your choice: ")) {
            std::cout << "Invalid choice!\n\n";
            continue;
        }
        logger.logUserInput(std::to_string(fieldChoice));

        switch (fieldChoice) {
        case 1: {
            std::string newName;
            if (!isValidInput(newName, "Enter new name: ")) {
                std::cout << "Invalid name!\n\n";
                break;
            }
            pipe.setName(newName);
            logger.logUserInput(newName);
            std::cout << "Name updated successfully!\n\n";
            break;
        }

        case 2: {
            float newLength;
            if (!isValidInput(newLength, "Enter new length: ")) {
                std::cout << "Invalid length!\n\n";
                break;
            }
            if (newLength <= 0) {
                std::cout << "Length must be positive!\n\n";
                break;
            }
            pipe.setLength(newLength);
            logger.logUserInput(std::to_string(newLength));
            std::cout << "Length updated successfully!\n\n";
            break;
        }

        case 3: {
            int newDiameter;
            if (!isValidInput(newDiameter, "Enter new diameter: ")) {
                std::cout << "Invalid diameter!\n\n";
                break;
            }
            if (newDiameter <= 0) {
                std::cout << "Diameter must be positive!\n\n";
                break;
            }
            pipe.setDiameter(newDiameter);
            logger.logUserInput(std::to_string(newDiameter));
            std::cout << "Diameter updated successfully!\n\n";
            break;
        }

        case 4: {
            std::cout << "Current status: " << (pipe.getStatus() ? "UNDER RENOVATION" : "WORKING") << "\n";
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
                return;
            }

            pipe.setStatus(newStatus);
            std::cout << "Repair status updated to: " << statusText << "\n\n";
            break;
        }

        case 0:
            std::cout << "Editing completed.\n\n";
            break;

        default:
            std::cout << "Invalid choice! Please enter 0-4.\n\n";
            break;
        }
    } while (fieldChoice != 0);
}

void editCompressStation(std::map<int, CompressStation>& stations, Logger& logger) {
    if (stations.empty()) {
        std::cout << "No compress stations available to edit.\n\n";
        return;
    }

    int stationId;
    if (!isValidInput(stationId, "Enter CS ID to edit: ")) {
        std::cout << "Invalid ID!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(stationId));

    if (stations.find(stationId) == stations.end()) {
        std::cout << "CS with ID " << stationId << " not found.\n\n";
        return;
    }

    CompressStation& station = stations[stationId];

    std::cout << "\nCurrent CS data:\n";
    std::cout << station;

    int fieldChoice;
    do {
        std::cout << "Select field to edit:\n";
        std::cout << "1. Name (current: " << station.getName() << ")\n";
        std::cout << "2. Number of workshops (current: " << station.getNumberOfWorkshops() << ")\n";
        std::cout << "3. Number of workshops in work (current: " << station.getNumberOfWorkshopsInWork() << ")\n";
        std::cout << "4. Class (current: " << station.getClass() << ")\n";
        std::cout << "0. Finish editing\n";

        if (!isValidInput(fieldChoice, "Enter your choice: ")) {
            std::cout << "Invalid choice!\n\n";
            continue;
        }
        logger.logUserInput(std::to_string(fieldChoice));

        switch (fieldChoice) {
        case 1: {
            std::string newName;
            if (!isValidInput(newName, "Enter new name: ")) {
                std::cout << "Invalid name!\n\n";
                break;
            }
            station.setName(newName);
            logger.logUserInput(newName);
            std::cout << "Name updated successfully!\n\n";
            break;
        }

        case 2: {
            int newWorkshops;
            if (!isValidInput(newWorkshops, "Enter new number of workshops: ")) {
                std::cout << "Invalid number!\n\n";
                break;
            }
            if (newWorkshops <= 0) {
                std::cout << "Number of workshops must be positive!\n\n";
                break;
            }

            if (station.getNumberOfWorkshopsInWork() > newWorkshops) {
                std::cout << "Warning: Current number of workshops in work ("
                    << station.getNumberOfWorkshopsInWork()
                    << ") exceeds new total (" << newWorkshops
                    << "). Please update workshops in work first.\n\n";
                break;
            }

            station.setNumberOfWorkshops(newWorkshops);
            logger.logUserInput(std::to_string(newWorkshops));
            std::cout << "Number of workshops updated successfully!\n\n";
            break;
        }

        case 3: {
            int newWorkshopsInWork;
            if (!isValidInput(newWorkshopsInWork, "Enter new number of workshops in work: ")) {
                std::cout << "Invalid number!\n\n";
                break;
            }

            if (newWorkshopsInWork > station.getNumberOfWorkshops()) {
                std::cout << "Error: Number of workshops in work cannot exceed total number of workshops ("
                    << station.getNumberOfWorkshops() << ").\n\n";
                break;
            }

            if (newWorkshopsInWork < 0) {
                std::cout << "Error: Number of workshops in work cannot be negative.\n\n";
                break;
            }

            station.setNumberOfWorkshopsInWork(newWorkshopsInWork);
            logger.logUserInput(std::to_string(newWorkshopsInWork));
            std::cout << "Number of workshops in work updated successfully!\n\n";
            break;
        }

        case 4: {
            std::string newClass;
            if (!isValidInput(newClass, "Enter new class: ")) {
                std::cout << "Invalid class!\n\n";
                break;
            }
            station.setClass(newClass);
            logger.logUserInput(newClass);
            std::cout << "Class updated successfully!\n\n";
            break;
        }

        case 0:
            std::cout << "Editing completed.\n\n";
            break;

        default:
            std::cout << "Invalid choice! Please enter 0-4.\n\n";
            break;
        }
    } while (fieldChoice != 0);
}

void batchEditPipes(std::map<int, Pipe>& pipes, Logger& logger) {
    if (pipes.empty()) {
        std::cout << "No pipes available to edit.\n\n";
        return;
    }

    std::cout << "Batch edit pipes - search pipes to edit:\n"
        << "1. By ID\n"
        << "2. By name\n"
        << "3. By repair status\n";

    int searchChoice;
    if (!isValidInput(searchChoice, "Enter your choice: ")) {
        std::cout << "Invalid choice!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(searchChoice));

    std::map<int, Pipe> foundPipes;

    if (searchChoice == 1) {
        int searchId;
        if (!isValidInput(searchId, "Enter pipe ID to search: ")) {
            std::cout << "Invalid ID!\n\n";
            return;
        }
        logger.logUserInput(std::to_string(searchId));
        foundPipes = findPipesById(pipes, searchId);

    }
    else if (searchChoice == 2) {
        std::string name;
        if (!isValidInput(name, "Enter name to search: ")) {
            std::cout << "Invalid name!\n\n";
            return;
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
            return;
        }
        logger.logUserInput(std::to_string(statusChoice));

        foundPipes = findPipesByStatus(pipes, statusChoice == 1);

    }
    else {
        std::cout << "Invalid choice!\n\n";
        return;
    }

    if (foundPipes.empty()) {
        std::cout << "No pipes to edit.\n\n";
        return;
    }

    std::cout << "\nFound " << foundPipes.size() << " pipes:\n\n";
    for (const auto& pair : foundPipes) {
        std::cout << "ID: " << pair.first << " - " << pair.second.getName()
            << " - " << (pair.second.getStatus() ? "UNDER RENOVATION" : "WORKING") << "\n";
    }

    std::cout << "\nChoose editing option:\n";
    std::cout << "1. Edit all found pipes\n";
    std::cout << "2. Select specific pipes to edit\n";
    std::cout << "3. Cancel\n";

    int choice;
    if (!isValidInput(choice, "Enter your choice: ")) {
        std::cout << "Invalid choice!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(choice));

    std::map<int, Pipe> pipesToEdit;

    if (choice == 1) {
        pipesToEdit = foundPipes;
    }
    else if (choice == 2) {
        std::cout << "Enter pipe IDs to edit (separated by spaces, 0 to finish):\n";
        int id;
        while (std::cin >> id && id != 0) {
            if (foundPipes.find(id) != foundPipes.end()) {
                pipesToEdit[id] = foundPipes[id];
                logger.logUserInput(std::to_string(id));
            }
            else {
                std::cout << "ID " << id << " not in found pipes.\n";
            }
        }
        clearInputBuffer();
    }
    else {
        std::cout << "Operation cancelled.\n\n";
        return;
    }

    if (pipesToEdit.empty()) {
        std::cout << "No pipes selected for editing.\n\n";
        return;
    }

    std::cout << "\nChoose what to edit:\n";
    std::cout << "1. Change repair status\n";
    std::cout << "2. Delete pipes\n";

    if (!isValidInput(choice, "Enter your choice: ")) {
        std::cout << "Invalid choice!\n\n";
        return;
    }
    logger.logUserInput(std::to_string(choice));

    if (choice == 1) {
        std::cout << "\nCurrent status of selected pipes:\n";
        for (const auto& pair : pipesToEdit) {
            std::cout << "ID: " << pair.first << " - " << pair.second.getName()
                << " - " << (pair.second.getStatus() ? "UNDER RENOVATION" : "WORKING") << "\n";
        }

        std::cout << "\nSet repair status for all selected pipes:\n";
        std::cout << "1. Put UNDER RENOVATION (repair ON)\n";
        std::cout << "2. Set to WORKING (repair OFF)\n";
        std::cout << "3. TOGGLE status (reverse current status)\n";
        std::cout << "0. Cancel\n";

        int statusChoice;
        if (!isValidInput(statusChoice, "Enter your choice: ")) {
            std::cout << "Invalid choice!\n\n";
            return;
        }
        logger.logUserInput(std::to_string(statusChoice));

        if (statusChoice == 0) {
            std::cout << "Operation cancelled.\n\n";
            return;
        }

        int changedCount = 0;

        switch (statusChoice) {
        case 1: {
            for (const auto& pair : pipesToEdit) {
                if (!pipes[pair.first].getStatus()) {
                    pipes[pair.first].setStatus(true);
                    changedCount++;
                }
            }
            std::cout << "Put " << changedCount << " pipes UNDER RENOVATION.\n\n";
            break;
        }

        case 2: {
            for (const auto& pair : pipesToEdit) {
                if (pipes[pair.first].getStatus()) {
                    pipes[pair.first].setStatus(false);
                    changedCount++;
                }
            }
            std::cout << "Set " << changedCount << " pipes to WORKING status.\n\n";
            break;
        }

        case 3: {
            for (const auto& pair : pipesToEdit) {
                pipes[pair.first].setStatus(!pipes[pair.first].getStatus());
                changedCount++;
            }
            std::cout << "Toggled status for " << changedCount << " pipes.\n\n";
            break;
        }

        default:
            std::cout << "Invalid choice! Operation cancelled.\n\n";
            break;
        }
    }
    else if (choice == 2) {
        std::cout << "Are you sure you want to delete " << pipesToEdit.size() << " pipes? (Y/N): ";
        char confirm;
        std::cin >> confirm;
        clearInputBuffer();
        logger.logUserInput(std::string(1, confirm));

        if (tolower(confirm) == 'y') {
            for (const auto& pair : pipesToEdit) {
                pipes.erase(pair.first);
            }
            std::cout << "Deleted " << pipesToEdit.size() << " pipes.\n\n";
        }
        else {
            std::cout << "Deletion cancelled.\n\n";
        }
    }
    else {
        std::cout << "Invalid choice!\n\n";
    }
}