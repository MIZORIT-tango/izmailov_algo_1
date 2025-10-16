#include "operations.h"
#include <algorithm>

void addPipe(std::map<int, Pipe>& pipes, Logger& logger) {
    std::string name;
    float length;
    int diameter;

    logger.logCommand("PIPE_NAME_INPUT");
    if (!isValidInput(name, "Enter name of pipe:\n")) {
        std::cout << "Invalid name!\n\n";
        logger.logUserInput("INVALID_NAME");
        return;
    }
    logger.logUserInput(name);

    logger.logCommand("PIPE_LENGTH_INPUT");
    if (!isValidInput(length, "Enter length of pipe:\n")) {
        std::cout << "Invalid length!\n\n";
        logger.logUserInput("INVALID_LENGTH");
        return;
    }
    logger.logUserInput(std::to_string(length));

    logger.logCommand("PIPE_DIAMETER_INPUT");
    if (!isValidInput(diameter, "Enter diameter of pipe:\n")) {
        std::cout << "Invalid diameter!\n\n";
        logger.logUserInput("INVALID_DIAMETER");
        return;
    }
    logger.logUserInput(std::to_string(diameter));

    int nextId = getNextPipeId(pipes);
    Pipe newPipe(nextId, name, length, diameter);
    pipes[newPipe.getId()] = newPipe;
    std::cout << "Pipe created successfully! ID: " << newPipe.getId() << "\n\n";
    logger.logCommand("PIPE_CREATED: ID=" + std::to_string(newPipe.getId()));
}

void addCompressStation(std::map<int, CompressStation>& stations, Logger& logger) {
    std::string name, class_cs;
    int workshops, workshops_in_work;

    logger.logCommand("CS_NAME_INPUT");
    if (!isValidInput(name, "Enter name of CS:\n")) {
        std::cout << "Invalid name!\n\n";
        logger.logUserInput("INVALID_NAME");
        return;
    }
    logger.logUserInput(name);

    logger.logCommand("CS_WORKSHOPS_INPUT");
    if (!isValidInput(workshops, "Enter number of workshops:\n")) {
        std::cout << "Invalid number!\n\n";
        logger.logUserInput("INVALID_WORKSHOPS");
        return;
    }
    logger.logUserInput(std::to_string(workshops));

    logger.logCommand("CS_WORKSHOPS_IN_WORK_INPUT");
    if (!isValidInput(workshops_in_work, "Enter number of workshops in work:\n")) {
        std::cout << "Invalid number!\n\n";
        logger.logUserInput("INVALID_WORKSHOPS_IN_WORK");
        return;
    }
    logger.logUserInput(std::to_string(workshops_in_work));

    if (workshops_in_work > workshops || workshops_in_work < 0) {
        std::cout << "Invalid number of workshops in work!\n\n";
        logger.logUserInput("INVALID_WORKSHOPS_COUNT");
        return;
    }

    logger.logCommand("CS_CLASS_INPUT");
    if (!isValidInput(class_cs, "Enter class of CS:\n")) {
        std::cout << "Invalid class!\n\n";
        logger.logUserInput("INVALID_CLASS");
        return;
    }
    logger.logUserInput(class_cs);

    int nextId = getNextStationId(stations);
    CompressStation newCS(nextId, name, workshops, workshops_in_work, class_cs);
    stations[newCS.getId()] = newCS;
    std::cout << "CS created successfully! ID: " << newCS.getId() << "\n\n";
    logger.logCommand("CS_CREATED: ID=" + std::to_string(newCS.getId()));
}

void viewAllObjects(const std::map<int, Pipe>& pipes, const std::map<int, CompressStation>& stations) {
    std::cout << "=== PIPES ===\n";
    if (pipes.empty()) {
        std::cout << "No pipes available.\n";
    }
    else {
        for (const auto& pair : pipes) {
            pair.second.displayInfo();
        }
    }

    std::cout << "=== COMPRESS STATIONS ===\n";
    if (stations.empty()) {
        std::cout << "No compress stations available.\n";
    }
    else {
        for (const auto& pair : stations) {
            pair.second.displayInfo();
        }
    }
    std::cout << std::endl;
}

std::vector<int> findPipesByName(const std::map<int, Pipe>& pipes, const std::string& name) {
    std::vector<int> result;
    for (const auto& pair : pipes) {
        if (pair.second.getName().find(name) != std::string::npos) {
            result.push_back(pair.first);
        }
    }
    return result;
}

std::vector<int> findPipesByStatus(const std::map<int, Pipe>& pipes, bool status) {
    std::vector<int> result;
    for (const auto& pair : pipes) {
        if (pair.second.getStatus() == status) {
            result.push_back(pair.first);
        }
    }
    return result;
}

std::vector<int> findStationsByName(const std::map<int, CompressStation>& stations, const std::string& name) {
    std::vector<int> result;
    for (const auto& pair : stations) {
        if (pair.second.getName().find(name) != std::string::npos) {
            result.push_back(pair.first);
        }
    }
    return result;
}

std::vector<int> findStationsByUnusedPercentage(const std::map<int, CompressStation>& stations, double minPercent, double maxPercent) {
    std::vector<int> result;
    for (const auto& pair : stations) {
        double percent = pair.second.getUnusedPercentage();
        if (percent >= minPercent && percent <= maxPercent) {
            result.push_back(pair.first);
        }
    }
    return result;
}

void editPipe(std::map<int, Pipe>& pipes, Logger& logger) {
    if (pipes.empty()) {
        std::cout << "No pipes available to edit.\n\n";
        logger.logCommand("EDIT_PIPE_NO_PIPES");
        return;
    }

    logger.logCommand("EDIT_PIPE_ID_INPUT");
    int pipeId;
    if (!isValidInput(pipeId, "Enter pipe ID to edit: ")) {
        std::cout << "Invalid ID!\n\n";
        logger.logUserInput("INVALID_PIPE_ID");
        return;
    }
    logger.logUserInput(std::to_string(pipeId));

    if (pipes.find(pipeId) == pipes.end()) {
        std::cout << "Pipe with ID " << pipeId << " not found.\n\n";
        logger.logCommand("PIPE_NOT_FOUND: ID=" + std::to_string(pipeId));
        return;
    }

    Pipe& pipe = pipes[pipeId];

    std::cout << "\nCurrent pipe data:\n";
    pipe.displayInfo();

    int fieldChoice;
    do {
        std::cout << "Select field to edit:\n";
        std::cout << "1. Name (current: " << pipe.getName() << ")\n";
        std::cout << "2. Length (current: " << pipe.getLength() << ")\n";
        std::cout << "3. Diameter (current: " << pipe.getDiameter() << ")\n";
        std::cout << "4. Repair status (current: " << (pipe.getStatus() ? "Under renovation" : "Working") << ")\n";
        std::cout << "0. Finish editing\n";

        logger.logCommand("EDIT_PIPE_FIELD_CHOICE");
        if (!isValidInput(fieldChoice, "Enter your choice: ")) {
            std::cout << "Invalid choice!\n\n";
            logger.logUserInput("INVALID_FIELD_CHOICE");
            continue;
        }
        logger.logUserInput(std::to_string(fieldChoice));

        switch (fieldChoice) {
        case 1: {
            // –едактирование имени
            logger.logCommand("EDIT_PIPE_NEW_NAME");
            std::string newName;
            if (!isValidInput(newName, "Enter new name: ")) {
                std::cout << "Invalid name!\n\n";
                logger.logUserInput("INVALID_NEW_NAME");
                break;
            }
            pipe.setName(newName);
            logger.logUserInput(newName);
            std::cout << "Name updated successfully!\n\n";
            logger.logCommand("PIPE_NAME_UPDATED: ID=" + std::to_string(pipeId));
            break;
        }

        case 2: {
            // –едактирование длины
            logger.logCommand("EDIT_PIPE_NEW_LENGTH");
            float newLength;
            if (!isValidInput(newLength, "Enter new length: ")) {
                std::cout << "Invalid length!\n\n";
                logger.logUserInput("INVALID_NEW_LENGTH");
                break;
            }
            if (newLength <= 0) {
                std::cout << "Length must be positive!\n\n";
                logger.logUserInput("INVALID_LENGTH_VALUE");
                break;
            }
            pipe.setLength(newLength);
            logger.logUserInput(std::to_string(newLength));
            std::cout << "Length updated successfully!\n\n";
            logger.logCommand("PIPE_LENGTH_UPDATED: ID=" + std::to_string(pipeId));
            break;
        }

        case 3: {
            // –едактирование диаметра
            logger.logCommand("EDIT_PIPE_NEW_DIAMETER");
            int newDiameter;
            if (!isValidInput(newDiameter, "Enter new diameter: ")) {
                std::cout << "Invalid diameter!\n\n";
                logger.logUserInput("INVALID_NEW_DIAMETER");
                break;
            }
            if (newDiameter <= 0) {
                std::cout << "Diameter must be positive!\n\n";
                logger.logUserInput("INVALID_DIAMETER_VALUE");
                break;
            }
            pipe.setDiameter(newDiameter);
            logger.logUserInput(std::to_string(newDiameter));
            std::cout << "Diameter updated successfully!\n\n";
            logger.logCommand("PIPE_DIAMETER_UPDATED: ID=" + std::to_string(pipeId));
            break;
        }

        case 4: {
            // –едактирование статуса ремонта
            logger.logCommand("EDIT_PIPE_NEW_STATUS");
            std::cout << "Current status: " << (pipe.getStatus() ? "UNDER RENOVATION" : "WORKING") << "\n";
            std::cout << "Select new repair status:\n";
            std::cout << "1. Put UNDER RENOVATION (repair ON)\n";
            std::cout << "2. Set to WORKING (repair OFF)\n";

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
            default:
                std::cout << "Invalid choice!\n\n";
                logger.logUserInput("INVALID_STATUS_VALUE");
                return;
            }

            pipe.setStatus(newStatus);
            std::cout << "Repair status updated to: " << statusText << "\n\n";
            logger.logCommand("PIPE_STATUS_UPDATED: ID=" + std::to_string(pipeId) +
                ", status=" + statusText);
            break;
        }

        case 0:
            logger.logCommand("EDIT_PIPE_FINISHED");
            std::cout << "Editing completed.\n\n";
            break;

        default:
            std::cout << "Invalid choice! Please enter 0-4.\n\n";
            logger.logUserInput("INVALID_EDIT_CHOICE");
            break;
        }
    } while (fieldChoice != 0);
}


void editCompressStation(std::map<int, CompressStation>& stations, Logger& logger) {
    if (stations.empty()) {
        std::cout << "No compress stations available to edit.\n\n";
        logger.logCommand("EDIT_CS_NO_STATIONS");
        return;
    }

    logger.logCommand("EDIT_CS_ID_INPUT");
    int stationId;
    if (!isValidInput(stationId, "Enter CS ID to edit: ")) {
        std::cout << "Invalid ID!\n\n";
        logger.logUserInput("INVALID_CS_ID");
        return;
    }
    logger.logUserInput(std::to_string(stationId));

    if (stations.find(stationId) == stations.end()) {
        std::cout << "CS with ID " << stationId << " not found.\n\n";
        logger.logCommand("CS_NOT_FOUND: ID=" + std::to_string(stationId));
        return;
    }

    CompressStation& station = stations[stationId]; 

    std::cout << "\nCurrent CS data:\n";
    station.displayInfo();

    int fieldChoice;
    do {
        std::cout << "Select field to edit:\n";
        std::cout << "1. Name (current: " << station.getName() << ")\n";
        std::cout << "2. Number of workshops (current: " << station.getNumberOfWorkshops() << ")\n";
        std::cout << "3. Number of workshops in work (current: " << station.getNumberOfWorkshopsInWork() << ")\n";
        std::cout << "4. Class (current: " << station.getClass() << ")\n";
        std::cout << "0. Finish editing\n";

        logger.logCommand("EDIT_CS_FIELD_CHOICE");
        if (!isValidInput(fieldChoice, "Enter your choice: ")) {
            std::cout << "Invalid choice!\n\n";
            logger.logUserInput("INVALID_FIELD_CHOICE");
            continue;
        }
        logger.logUserInput(std::to_string(fieldChoice));

        switch (fieldChoice) {
        case 1: {
            // –едактирование имени
            logger.logCommand("EDIT_CS_NEW_NAME");
            std::string newName;
            if (!isValidInput(newName, "Enter new name: ")) {
                std::cout << "Invalid name!\n\n";
                logger.logUserInput("INVALID_NEW_NAME");
                break;
            }
            station.setName(newName);
            logger.logUserInput(newName);
            std::cout << "Name updated successfully!\n\n";
            logger.logCommand("CS_NAME_UPDATED: ID=" + std::to_string(stationId));
            break;
        }

        case 2: {
            // –едактирование общего количества цехов
            logger.logCommand("EDIT_CS_NEW_WORKSHOPS");
            int newWorkshops;
            if (!isValidInput(newWorkshops, "Enter new number of workshops: ")) {
                std::cout << "Invalid number!\n\n";
                logger.logUserInput("INVALID_NEW_WORKSHOPS");
                break;
            }
            if (newWorkshops <= 0) {
                std::cout << "Number of workshops must be positive!\n\n";
                logger.logUserInput("INVALID_WORKSHOPS_VALUE");
                break;
            }

            // ѕровер€ем чтобы работающих цехов не было больше нового общего количества
            if (station.getNumberOfWorkshopsInWork() > newWorkshops) {
                std::cout << "Warning: Current number of workshops in work ("
                    << station.getNumberOfWorkshopsInWork()
                    << ") exceeds new total (" << newWorkshops
                    << "). Please update workshops in work first.\n\n";
                logger.logCommand("WORKSHOPS_WARNING: in_work_exceeds_total");
                break;
            }

            station.setNumberOfWorkshops(newWorkshops);
            logger.logUserInput(std::to_string(newWorkshops));
            std::cout << "Number of workshops updated successfully!\n\n";
            logger.logCommand("CS_WORKSHOPS_UPDATED: ID=" + std::to_string(stationId));
            break;
        }

        case 3: {
            // –едактирование количества работающих цехов
            logger.logCommand("EDIT_CS_NEW_WORKSHOPS_IN_WORK");
            int newWorkshopsInWork;
            if (!isValidInput(newWorkshopsInWork, "Enter new number of workshops in work: ")) {
                std::cout << "Invalid number!\n\n";
                logger.logUserInput("INVALID_NEW_WORKSHOPS_IN_WORK");
                break;
            }

            // ѕровер€ем чтобы работающих цехов не было больше общего количества
            if (newWorkshopsInWork > station.getNumberOfWorkshops()) {
                std::cout << "Error: Number of workshops in work cannot exceed total number of workshops ("
                    << station.getNumberOfWorkshops() << ").\n\n";
                logger.logUserInput("INVALID_WORKSHOPS_IN_WORK_COUNT");
                break;
            }

            if (newWorkshopsInWork < 0) {
                std::cout << "Error: Number of workshops in work cannot be negative.\n\n";
                logger.logUserInput("INVALID_WORKSHOPS_IN_WORK_VALUE");
                break;
            }

            station.setNumberOfWorkshopsInWork(newWorkshopsInWork);
            logger.logUserInput(std::to_string(newWorkshopsInWork));
            std::cout << "Number of workshops in work updated successfully!\n\n";
            logger.logCommand("CS_WORKSHOPS_IN_WORK_UPDATED: ID=" + std::to_string(stationId));
            break;
        }

        case 4: {
            // –едактирование класса
            logger.logCommand("EDIT_CS_NEW_CLASS");
            std::string newClass;
            if (!isValidInput(newClass, "Enter new class: ")) {
                std::cout << "Invalid class!\n\n";
                logger.logUserInput("INVALID_NEW_CLASS");
                break;
            }
            station.setClass(newClass);
            logger.logUserInput(newClass);
            std::cout << "Class updated successfully!\n\n";
            logger.logCommand("CS_CLASS_UPDATED: ID=" + std::to_string(stationId));
            break;
        }

        case 0:
            logger.logCommand("EDIT_CS_FINISHED");
            std::cout << "Editing completed.\n\n";
            break;

        default:
            std::cout << "Invalid choice! Please enter 0-4.\n\n";
            logger.logUserInput("INVALID_EDIT_CHOICE");
            break;
        }
    } while (fieldChoice != 0);
}

void batchEditPipes(std::map<int, Pipe>& pipes, const std::vector<int>& pipeIds, Logger& logger) {
    if (pipeIds.empty()) {
        std::cout << "No pipes to edit.\n\n";
        logger.logCommand("BATCH_EDIT_NO_PIPES");
        return;
    }

    std::cout << "Found " << pipeIds.size() << " pipes:\n";
    for (int id : pipeIds) {
        std::cout << "ID: " << id << " - " << pipes[id].getName() << "\n";
    }

    logger.logCommand("BATCH_EDIT_OPTIONS");
    std::cout << "\nChoose editing option:\n";
    std::cout << "1. Edit all found pipes\n";
    std::cout << "2. Select specific pipes to edit\n";
    std::cout << "3. Cancel\n";

    int choice;
    if (!isValidInput(choice, "Enter your choice: ")) {
        std::cout << "Invalid choice!\n\n";
        logger.logUserInput("INVALID_CHOICE");
        return;
    }
    logger.logUserInput(std::to_string(choice));

    std::vector<int> pipesToEdit;

    if (choice == 1) {
        pipesToEdit = pipeIds;
        logger.logCommand("BATCH_EDIT_ALL_SELECTED");
    }
    else if (choice == 2) {
        logger.logCommand("BATCH_EDIT_SELECT_SPECIFIC");
        std::cout << "Enter pipe IDs to edit (separated by spaces, 0 to finish):\n";
        int id;
        while (std::cin >> id && id != 0) {
            if (std::find(pipeIds.begin(), pipeIds.end(), id) != pipeIds.end()) {
                pipesToEdit.push_back(id);
                logger.logUserInput(std::to_string(id));
            }
            else {
                std::cout << "ID " << id << " not in found pipes.\n";
                logger.logUserInput("INVALID_ID: " + std::to_string(id));
            }
        }
        clearInputBuffer();
    }
    else {
        std::cout << "Operation cancelled.\n\n";
        logger.logCommand("BATCH_EDIT_CANCELLED");
        return;
    }

    if (pipesToEdit.empty()) {
        std::cout << "No pipes selected for editing.\n\n";
        logger.logCommand("BATCH_EDIT_NO_SELECTION");
        return;
    }

    logger.logCommand("BATCH_EDIT_ACTION");
    std::cout << "\nChoose what to edit:\n";
    std::cout << "1. Change repair status\n";
    std::cout << "2. Delete pipes\n";

    if (!isValidInput(choice, "Enter your choice: ")) {
        std::cout << "Invalid choice!\n\n";
        logger.logUserInput("INVALID_ACTION_CHOICE");
        return;
    }
    logger.logUserInput(std::to_string(choice));

    if (choice == 1) {
        logger.logCommand("BATCH_EDIT_CHANGE_STATUS");

        std::cout << "\nCurrent status of selected pipes:\n";
        for (int id : pipesToEdit) {
            std::cout << "ID: " << id << " - " << pipes[id].getName()
                << " - " << (pipes[id].getStatus() ? "UNDER RENOVATION" : "WORKING") << "\n";
        }

        std::cout << "\nSet repair status for all selected pipes:\n";
        std::cout << "1. Put UNDER RENOVATION (repair ON)\n";
        std::cout << "2. Set to WORKING (repair OFF)\n";
        std::cout << "3. TOGGLE status (reverse current status)\n";
        std::cout << "0. Cancel\n";

        int statusChoice;
        if (!isValidInput(statusChoice, "Enter your choice: ")) {
            std::cout << "Invalid choice!\n\n";
            logger.logUserInput("INVALID_STATUS_CHOICE");
            return;
        }
        logger.logUserInput(std::to_string(statusChoice));

        if (statusChoice == 0) {
            std::cout << "Operation cancelled.\n\n";
            logger.logCommand("BATCH_STATUS_CANCELLED");
            return;
        }

        int changedCount = 0;

        switch (statusChoice) {
        case 1: {
            for (int id : pipesToEdit) {
                if (!pipes[id].getStatus()) {
                    pipes[id].setStatus(true);
                    changedCount++;
                }
            }
            std::cout << "Put " << changedCount << " pipes UNDER RENOVATION.\n\n";
            logger.logCommand("BATCH_STATUS_SET_RENOVATION: count=" + std::to_string(changedCount));
            break;
        }

        case 2: {
            for (int id : pipesToEdit) {
                if (pipes[id].getStatus()) {
                    pipes[id].setStatus(false);
                    changedCount++;
                }
            }
            std::cout << "Set " << changedCount << " pipes to WORKING status.\n\n";
            logger.logCommand("BATCH_STATUS_SET_WORKING: count=" + std::to_string(changedCount));
            break;
        }

        case 3: {
            for (int id : pipesToEdit) {
                pipes[id].setStatus(!pipes[id].getStatus());
                changedCount++;
            }
            std::cout << "Toggled status for " << changedCount << " pipes.\n\n";
            logger.logCommand("BATCH_STATUS_TOGGLED: count=" + std::to_string(changedCount));
            break;
        }

        default:
            std::cout << "Invalid choice! Operation cancelled.\n\n";
            logger.logUserInput("INVALID_STATUS_ACTION");
            break;
        }
    }
    else if (choice == 2) {
        logger.logCommand("BATCH_EDIT_DELETE_PIPES");
        for (int id : pipesToEdit) {
            pipes.erase(id);
        }
        std::cout << "Deleted " << pipesToEdit.size() << " pipes.\n\n";
        logger.logCommand("BATCH_PIPES_DELETED: count=" + std::to_string(pipesToEdit.size()));
    }
    else {
        std::cout << "Invalid choice!\n\n";
        logger.logUserInput("INVALID_BATCH_ACTION");
    }
}