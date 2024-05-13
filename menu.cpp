#include <iostream>
#include <string>

#include "functions.h"

void displayHelp() {
    std::cout << "\nCommand list: \n";
    std::cout << "help          displays commands\n";
    std::cout << "marquee       display marquee console\n";
    std::cout << "process-smi   display process SMI\n";
    std::cout << "nvidia-smi    display nvidia SMI\n";
    std::cout << "screen        view process-specific screen\n";
    std::cout << "clear         clear screen\n";
    std::cout << "exit          exit OS\n";
}
void printHeader() {
    std::cout << "------------------------------------- \n";
    std::cout << "Welcome to CSOPESY Group 3's Lite OS! \n";
    std::cout << "\n _____ _____ _____ _____ _____ _____ __ __    _____     _       _    _____ _____ \n";
    std::cout << "|     |   __|     |  _  |   __|   __|  |  |  |   __|___| |_ ___|_|  |     |   __|\n";
    std::cout << "|   --|__   |  |  |   __|   __|__   |_   _|  |  |  |___|  _|  _| |  |  |  |__   |\n";
    std::cout << "|_____|_____|_____|__|  |_____|_____| |_|    |_____|   |_| |_| |_|  |_____|_____|\n";
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