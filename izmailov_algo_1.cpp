// izmailov_algo_1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

struct Pipe {
    std::string name;
    float lenth;
    int diametr;
    bool status;
};

struct CompressStation {
    std::string name;
    int number_of_workshops;
    int number_of_workshops_in_work;
    std::string class_cs;
};

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
        std::cin >> menu_choose;

        switch (menu_choose) {
        case 1:

            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

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

