#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <regex>

struct Pipe {
    std::string name= "0";
    float length = 0.;
    int diameter = 0;
    bool status = false;

    bool switch_status(int o) {
        if (o == 1) {
            status = true;
            std::cout << "Status: Under renovation\n\n";
            return status;
        }
        else {
            if (o == 2) {
                status = false;
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

void show_menu(Pipe& p, CompressStation& c) {
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
        std::cin >> menu_choose;

        switch (menu_choose) {
        case 1:
            {
                bool type_error = false;  

                std::string safe_name;
                float safe_length = 0.0f;
                int safe_diameter = 0;

                std::cout << "Enter name of pipe:\n";
                clearInputBuffer();
                std::getline(std::cin, safe_name);

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

                if (!type_error) {
                    std::cout << "Enter diameter of pipe:\n";

                    std::string input;
                    std::getline(std::cin, input);
                    std::stringstream ss(input);
                    int value;
                    
                    if (!(ss >> value) || !ss.eof() || value <= 0) {
                        std::cout << "Invalid input! Please enter an integer over 0 (without decimal point).\n\n";
                        type_error = true;
                    }
                    else {
                        safe_diameter = value;
                    }
                }

                if (!type_error) {
                    p.name = safe_name;
                    p.length = safe_length;
                    p.diameter = safe_diameter;
                    std::cout << "Pipe created successfully!\n\n";
                }                
                clearInputBuffer();
            }
            break;

        case 2:
        {
            bool type_error = false;

            std::string safe_name;
            int safe_number_of_workshops = 0;
            int safe_number_of_workshops_in_work = 0;

            std::cout << "Enter name of CS\n";
            clearInputBuffer();
            std::getline(std::cin, safe_name);

            std::cout << "Enter number of workshops:\n";
            std::string input;
            std::getline(std::cin, input);
            std::stringstream ss(input);
            int value;
            if (!(ss >> value) || !ss.eof() || value <= 0) {
                std::cout << "Invalid input! Please enter an integer over 0 (without decimal point).\n\n";
                type_error = true;
            }
            else {
                safe_number_of_workshops = value;
            }

            if (!type_error) {
                std::cout << "Enter number of workshops in work:\n";
                std::string input;
                std::getline(std::cin, input);
                std::stringstream ss(input);
                int value;

                if (!(ss >> value) || !ss.eof() || value <= 0) {
                    std::cout << "Invalid input! Please enter an integer over 0 (without decimal point).\n\n";
                    type_error = true;
                }
                if (value > safe_number_of_workshops) {
                    std::cout << "Invalid input! number of workshops in work can`t be over than number of workshops\n\n";
                    type_error = true;
                }
                else {
                    safe_number_of_workshops_in_work = value;
                }
            }

            if (!type_error) {
                c.name = safe_name;
                c.number_of_workshops = safe_number_of_workshops;
                c.number_of_workshops_in_work = safe_number_of_workshops_in_work;
                std::cout << "CS created successfully!\n\n";
            }
            clearInputBuffer();

            break;
        }

        case 3:
            
            break;
        case 4:
            if (object_exist(p.name)) {
                std::cout << "select the operating mode of the pipe\n"
                    << "1. Repair ON\n"
                    << "2. Repair OFF\n";
                int op_mode_pipe;
                std::cin >> op_mode_pipe;
                p.switch_status(op_mode_pipe);
            }
            else {
                std::cout << "Pipe is not exist! Do it first.\n\n";
            }
            break;
        case 5:

            break;
        case 6:

            break;
        case 7:

            break;
        case 0:
            return;
        }
    }
}


int main()
{
    Pipe my_pipe;
    CompressStation CS;
    std::cout << "Welcome to the application 'basic essences of pipeline transportation of gas or oil'\n";
    show_menu(my_pipe, CS);
    
}

