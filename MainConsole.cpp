#include <iostream>
#include <string>

#include "functions.h"

int main() {

    printHeader();

    bool validInput = false;

    while (!validInput) {

        std::cout << "\nPlease enter command here (use help for a list of commands!): ";
        std::string command;
        std::getline(std::cin, command);
        
        if (command == "help") {
            displayHelp();
        }

        else if (command == "marquee") {
            marquee();
        }

        else if (command == "screen") {
            screen();
        }

        else if (command == "process-smi") {
            processSmi();
        }

        else if (command == "nvidia-smi") {
            nvidiaSmi();
        }

        else if (command == "clear") {
            clear();
        }

        else if (command == "exit") {
            exit();
            validInput = true;
        }

        else {
            std::cout << "Command not recognized.\n";
        }

    }

    return 0;
}