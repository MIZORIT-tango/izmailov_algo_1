#include <iostream>
#include <string>
#include <limits>

struct Pipe {
    std::string name= "0";
    float lenth = 0.;
    int diametr = 0;
    bool status = false;

    bool switch_status(int o) {
        if (o == 1) {
            status = true;
            std::cout << "Status: Under renovation\n";
            return status;
        }
        else {
            if (o == 2) {
                status = false;
                std::cout << "Status: normal\n";
                return status;
            }
            else {
                std::cout << "Incorrect value! Try again\n";
                std::cout << "Status is not changed\n";
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
            std::cout << "Enter name of pipe\n";
            std::getline(std::cin, p.name);
            break;

        case 2:
            std::cout << "Enter name of CS\n";
            std::getline(std::cin, c.name);
            break;

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

