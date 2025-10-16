#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <regex>
#include <cctype>
#include <fstream>
#include <map>

struct Pipe {
    std::string name= "0";
    float length = 0.;
    int diameter = 0;
    bool status = false;

    bool switch_status(int o) {
        if (o == 1) {
            status = true;
            std::cout << "Successfully!\n";
            std::cout << "Status: Under renovation\n\n";
            return status;
        }
        else {
            if (o == 2) {
                status = false;
                std::cout << "Successfully!\n";
                std::cout << "Status: normal\n\n";
                return status;
            }
            else {
                std::cout << "Incorrect value! Try again\n";
                std::cout << "Status is not changed\n\n";
                return status;
            }
        }
    }
};

struct CompressStation {
    std::string name = "0";
    int number_of_workshops = 0;
    int number_of_workshops_in_work = 0;
    std::string class_cs = "0";

    bool change_number_of_workshops_in_work(int number) {
        if ( number > number_of_workshops) {
            std::cout << "Invalid input! number of workshops in work can`t be over than number of workshops\n";
            std::cout << "number is not changed\n\n";
            return false;
        }
        else if (number < 0) {
            std::cout << "Invalid input! Please enter an integer over 0 (without decimal point).\n";
            std::cout << "number is not changed\n\n";
            return false;
        }
        else { 
            number_of_workshops_in_work = number;
            return true; 
        }
    }
};
 
bool object_exist(std::string object) {
    if (object == "0") {
        return false;
    }
    else {
        return true;
    }
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool isValidFloat(const std::string& input) {
    if (input.empty()) return false;

    int start = 0;
    bool has_decimal = false;
    bool has_digit = false;

    if (input[0] == '+' || input[0] == '-') {
        start = 1;
        if (input.size() == 1) return false;
    }

    for (int i = start; i < input.size(); i++) {
        char c = input[i];

        if (c >= '0' && c <= '9') {
            has_digit = true;
        }
        else if (c == '.' || c == ',') {
            if (has_decimal) return false;
            has_decimal = true;
        }
        else {
            return false;
        }
    }
    return has_digit;
}

bool isValidInt(int& result, bool comment=true, bool zero=false) {
    std::string input;
    std::getline(std::cin, input);
    std::stringstream ss(input);
    int value;

    if (!(ss >> value) || !ss.eof() || (value == 0 && !zero) || value < 0) {
        if (comment) {
            std::cout << "Invalid input! Please enter an positive integer (without decimal point).\n\n";
        }
        return false;
    }
    else {
        result = value;
        return true;
    }
}

std::string trim(const std::string& str, bool isalnum=false) {
    if (str.empty()) return "error_input";

    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    size_t end = str.find_last_not_of(" \t\n\r\f\v");

    if (start == std::string::npos) {
        return "error_input";
    }

    if (!isalnum) {
        bool has_alnum = false;
        for (size_t i = start; i <= end; i++) {
            if (std::isalnum(static_cast<unsigned char>(str[i]))) {
                has_alnum = true;
                break;
            }
        }
        if (has_alnum) {
            return str.substr(start, end - start + 1);
        }
        else {
            return "error_input";
        }
    }
     return str.substr(start, end - start + 1);
}

void saveData(const std::string& fileName, const Pipe& p, const CompressStation& c) {
    std::ofstream file(fileName);

    if (file.is_open()) {
        if (object_exist(p.name)) {
            file << "<PIPE>\n";
            file << p.name << "\n";
            file << p.length << "\n";
            file << p.diameter << "\n";
            file << p.status << "\n";
            file << "</PIPE>\n\n";
        }
        else {
            std::cout << "Pipe is not exist, not saved\n\n";
        }
        if (object_exist(c.name)) {
            file << "<CS>\n";
            file << c.name << "\n";
            file << c.number_of_workshops << "\n";
            file << c.number_of_workshops_in_work << "\n";
            file << c.class_cs << "\n";
            file << "</CS>\n\n";
        }
        else {
            std::cout << "CS is not exist, not saved\n\n";
        }
        file.close();
        std::cout << "Data saved to " << fileName << "\n" << std::endl;
    }
    else {
        std::cout << "Error save. File is not found.";
    }
}

void loadData(const std::string& filename, Pipe& p, CompressStation& c) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Error opening file for reading!" << std::endl << "\n\n";
        return;
    }

    std::string line;
    bool reading_pipe = false;
    bool reading_cs = false;
    int line_counter = 0;

    try {
        while (std::getline(file, line)) {
            line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
            line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

            if (line.empty()) {
                continue;
            }
            
            if (line == "<PIPE>") {
                if (reading_pipe || reading_cs) {
                    throw std::runtime_error("Nested tags found");
                }
                reading_pipe = true;
                line_counter = 0;
                continue;
            }
            else if (line == "</PIPE>") {
                if (!reading_pipe) {
                    throw std::runtime_error("Unexpected closing tag </PIPE>");
                }
                if (line_counter != 4) {
                    throw std::runtime_error("Incomplete PIPE data");
                }
                reading_pipe = false;
                line_counter = 0;
                continue;
            }
            else if (line == "<CS>") {
                if (reading_pipe || reading_cs) {
                    throw std::runtime_error("Nested tags found");
                }
                reading_cs = true;
                line_counter = 0;
                continue;
            }
            else if (line == "</CS>") {
                if (!reading_cs) {
                    throw std::runtime_error("Unexpected closing tag </CS>");
                }
                if (line_counter != 4) {
                    throw std::runtime_error("Incomplete CS data");
                }
                reading_cs = false;
                line_counter = 0;
                continue;
            }

            if (reading_pipe) {
                switch (line_counter) {
                case 0: p.name = line; break;
                case 1: p.length = std::stof(line); break;
                case 2: p.diameter = std::stoi(line); break;
                case 3: p.status = (line == "1" || line == "true"); break;
                default:
                    throw std::runtime_error("Extra data in PIPE tag");
                }
                line_counter++;
            }
            else if (reading_cs) {
                switch (line_counter) {
                case 0: c.name = line; break;
                case 1: c.number_of_workshops = std::stoi(line); break;
                case 2: c.number_of_workshops_in_work = std::stoi(line); break;
                case 3: c.class_cs = line; break;
                default:
                    throw std::runtime_error("Extra data in CS tag");
                }
                line_counter++;
            }
            else {
                throw std::runtime_error("Data outside of tags: " + line);
            }
        }

        if (reading_pipe) {
            throw std::runtime_error("Unclosed <PIPE> tag");
        }
        if (reading_cs) {
            throw std::runtime_error("Unclosed <CS> tag");
        }

        std::cout << "Data loaded successfully from " << filename << "\n" << std::endl;

    }
    catch (const std::exception& e) {
        std::cout << "Error reading file: " << e.what() << "\n" << std::endl;
        p = Pipe();
        c = CompressStation();
    }

    file.close();
}

void show_menu(Pipe p, CompressStation c) {
    int menu_choose; 
    while (true) {
        std::cout << "Select an action: (write number))\n"
            << "1. Add pipe\n"
            << "2. Add CS\n"
            << "3. Viewing all objects\n"
            << "4. Edit pipe\n"
            << "5. Edit CS\n"
            << "6. Save\n"
            << "7. Download\n"
            << "0. Exit\n";        

        try {
            std::string input;
            std::getline(std::cin, input);
            menu_choose = std::stoi(input);

            if (menu_choose >= 0 && menu_choose <= 7) {
                switch (menu_choose) {
                case 1:
                {
                    bool type_error = false;

                    std::string safe_name;
                    float safe_length = 0.0f;
                    int safe_diameter = 0;

                    std::cout << "Enter name of pipe:\n";
                    std::getline(std::cin, safe_name);
                    safe_name = trim(safe_name);
                    if (safe_name == "error_input") {
                        std::cout << "Invalid input! The name must contain at least one letter or number.\n\n";
                        type_error = true;
                    }

                    if (!type_error) {
                        std::cout << "Enter length of pipe:\n";
                        std::string input;
                        std::getline(std::cin, input);

                        if (!isValidFloat(input)) {
                            std::cout << "Invalid input! Please enter a float number without spaces.\n\n";
                            type_error = true;
                        }
                        else {
                            std::replace(input.begin(), input.end(), ',', '.');
                            safe_length = std::stof(input);
                            if (safe_length <= 0) {
                                std::cout << "Invalid input! Please enter a positive number.\n\n";
                                type_error = true;
                            }
                        }
                    }

                    if (!type_error) {
                        std::cout << "Enter diameter of pipe:\n";
                        if (!isValidInt(safe_diameter)) {
                            type_error = true;
                        }
                    }

                    if (!type_error) {
                        p.name = safe_name;
                        p.length = safe_length;
                        p.diameter = safe_diameter;
                        std::cout << "Pipe created successfully!\n\n";
                    }
                }
                break;

                case 2:
                {
                    bool type_error = false;

                    std::string safe_name;
                    std::string safe_class;
                    int safe_number_of_workshops = 0;
                    int safe_number_of_workshops_in_work = 0;

                    std::cout << "Enter name of CS\n";
                    std::getline(std::cin, safe_name);
                    safe_name = trim(safe_name);
                    if (safe_name == "error_input") {
                        std::cout << "Invalid input! The name must contain at least one letter or number.\n\n";
                        type_error = true;
                    }

                    if (!type_error) {
                        std::cout << "Enter number of workshops:\n";
                        if (!isValidInt(safe_number_of_workshops)) {
                            type_error = true;
                        }
                    }

                    if (!type_error) {
                        std::cout << "Enter number of workshops in work:\n";
                        if (!isValidInt(safe_number_of_workshops_in_work)) {
                            type_error = true;
                        }
                        else if (safe_number_of_workshops_in_work > safe_number_of_workshops) {
                            std::cout << "Invalid input! number of workshops in work can`t be over than number of workshops\n\n";
                            type_error = true;
                        }
                    }

                    if (!type_error) {
                        std::cout << "Enter Class of workshops:\n";
                        std::getline(std::cin, safe_class);
                        safe_class = trim(safe_class, true);
                        if (safe_class == "error_input") {
                            std::cout << "Invalid input! The class can`t be empty.\n\n";
                            type_error = true;
                        }
                        }                    

                    if (!type_error) {
                        c.name = safe_name;
                        c.number_of_workshops = safe_number_of_workshops;
                        c.number_of_workshops_in_work = safe_number_of_workshops_in_work;
                        c.class_cs = safe_class;
                        std::cout << "CS created successfully!\n\n";
                    }
                    break;
                }

                case 3:
                    if (object_exist(p.name)) {
                        std::cout << "INFO of pipe:\n";
                        std::cout << "Name: " << p.name << "\n";
                        std::cout << "length: " << p.length << "\n";
                        std::cout << "diameter: " << p.diameter << "\n";
                        if (p.status) {
                            std::cout << "Under renovation " << "\n\n";
                        }
                        else {
                            std::cout << "Working " << "\n\n";
                        }
                    }
                    else {
                        std::cout << "Pipe is not exists\n\n";
                    }

                    if (object_exist(c.name)) {
                        std::cout << "INFO of CS:\n";
                        std::cout << "Name: " << c.name << "\n";
                        std::cout << "Number of workshops: " << c.number_of_workshops << "\n";
                        std::cout << "Number of workshops in work: " << c.number_of_workshops_in_work << "\n";
                        std::cout << "Class: " << c.class_cs << "\n\n";
                    }
                    else {
                        std::cout << "CS is not exists\n\n";
                    }
                    break;

                case 4:
                    if (object_exist(p.name)) {
                        std::cout << "select the operating mode of the pipe\n"
                            << "1. Repair ON\n"
                            << "2. Repair OFF\n";
                        int op_mode_pipe;
                        if (!isValidInt(op_mode_pipe, false)) {
                            std::cout << "Input error, please enter integer\n\n";
                        }
                        else p.switch_status(op_mode_pipe);                        
                    }
                    else {
                        std::cout << "Pipe is not exist! Do it first.\n\n";
                    }
                    break;

                case 5:
                    if (object_exist(c.name)) {
                        std::cout << "_____number of workshops in work now is - " << c.number_of_workshops_in_work << "_____\n";
                        std::cout << "Keep track of how many workstations should be in operation:\n";                        
                        int CS_ws_change;
                        if (!isValidInt(CS_ws_change, true, true)) {
                            break;
                        }
                        else if (c.change_number_of_workshops_in_work(CS_ws_change)) {
                            std::cout << "Successfully\n";
                            std::cout << "_____number of workshops in work now is - " << c.number_of_workshops_in_work << "_____\n\n";
                        }
                    }
                    else {
                        std::cout << "CS is not exist! Do it first.\n\n";
                    }
                    break;
                case 6:
                    std::cout << "Are you sure? The file will be permanently replaced.\n";
                    std::cout << "choice: Y/N  (Yes/Not)\n";

                    char s_choice;
                    std::cin >> s_choice;
                    s_choice = tolower(s_choice);

                    if (s_choice == 'y') {
                        saveData("data_file.txt", p, c);
                        std::cout << "Saving completed\n\n";
                    }
                    else if (s_choice == 'n') {
                        std::cout << "\n\n";
                    }
                    else {
                        std::cout << "Error input\n\n" << std::endl;                        
                    }
                    clearInputBuffer();
                    break;

                case 7:
                    std::cout << "Are you sure? The original data will be delete.\n";
                    std::cout << "choice: Y/N  (Yes/Not)\n";

                    char l_choice;
                    std::cin >> l_choice;
                    l_choice = tolower(l_choice);

                    if (l_choice == 'y') {
                        loadData("data_file.txt", p, c);
                        std::cout << "Load completed\n\n";
                    }
                    else if (l_choice == 'n') {
                        std::cout << "\n";
                    }
                    else {
                        std::cout << "Error input\n\n" << std::endl;                        
                    }
                    clearInputBuffer();
                    break;

                case 0:
                    return;
                }
            }
            else {
                std::cout << "Number must be between 0 and 7. Try again.\n\n";
            }
        }
        catch (const std::exception&) {
            std::cout << "Invalid input! Please enter a number.\n\n";
            continue;            
        }
    }
}


int main()
{
    Pipe my_pipe;
    CompressStation CS;
    std::map<int, Pipe> pipes_massive;
    std::map<int, CompressStation> cs_massive;
    std::cout << "Welcome to the application 'basic essences of pipeline transportation of gas or oil'\n";
    show_menu(my_pipe, CS);  
}