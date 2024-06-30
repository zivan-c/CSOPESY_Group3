#include <iostream>
#include <string>

#include "functions.h"

void displayHelp() {
    std::cout << "\nCommand list: \n";
    std::cout << "help            displays commands\n";
    std::cout << "initialize      initialize processor configuration - must be called before any commands are recognized\n";
    std::cout << "marquee         display marquee console - requires initialization first\n";
    std::cout << "report-util     generating CPU utilization report - requires initialization first\n";
    std::cout << "scheduler-stop  generates batch of dummy processes for CPU scheduler. - requires intialization first\n";
    std::cout << "scheduler-test  stops generating dummy processes - requires initialization first\n";
    std::cout << "screen          view process-specific screen - requires initialization first\n";
    std::cout << "clear           clear screen\n";
    std::cout << "exit            exit OS\n";
}

void printHeader() {
    std::cout << "------------------------------------- \n";
    std::cout << "Welcome to CSOPESY Group 3's Lite OS! \n";
    std::cout << " _____ _____ _____ _____ _____ _____ __ __    _____     _       _    _____ _____ \n";
    std::cout << "|     |   __|     |  _  |   __|   __|  |  |  |   __|___| |_ ___|_|  |     |   __|\n";
    std::cout << "|   --|__   |  |  |   __|   __|__   |_   _|  |  |  |___|  _|  _| |  |  |  |__   |\n";
    std::cout << "|_____|_____|_____|__|  |_____|_____| |_|    |_____|   |_| |_| |_|  |_____|_____|\n";
    std::cout << "\nDevelopers:\nGarcia, Aurelio Rodolfo F.\nJacinto, Jon Piolo C.\nMalong, Merrick Zivan C.\nMartinez, Michelle Andrea H.\n";
    std::cout << "\nLast updated: ";
    std::cout << "\n------------------------------------- \n";
}

// exit function: Closes application/CLI
void exit() {
    std::cout << "Exiting the OS..." << std::endl;
    exit(0);
}

// clear function: Clears screen, reprinting the header texts.
void clear() {
    system("cls");
    printHeader();
}

void initialize() {
    std::cout << "\ninitialize command recognized. Doing something.\n";
}

void reportUtil() {
    std::cout << "\nReport generated at C:/csopesy-log.txt\n";
}