#pragma once
#include "MainMenu.h"
#include "Console.h"
#include "ConsoleManager.h"
#include "OSConfig.h"
#include "Scheduler.h"

MainMenu::MainMenu() : Console(MAINCON) {
}

void MainMenu::setConfig()
{
	OSConfig::initialize();
	OSConfig::readConfig();
}

void MainMenu::help() {
    std::cout << "\nCommand list: \n";
    std::cout << "help          displays commands\n";
    std::cout << "marquee       display marquee console\n";
    std::cout << "process-smi   display process SMI\n";
    std::cout << "nvidia-smi    display nvidia SMI\n";
    std::cout << "screen        view process-specific screen\n";
    std::cout << "clear         clear screen\n";
    std::cout << "exit          exit OS\n";
}

void MainMenu::printHeader() {
    if (isNew) {
        system("cls");
        ConsoleManager::getInst()->setCursorPosition(0, 0);
        this->isNew = false;

        std::cout << "------------------------------------- \n";
        std::cout << "Welcome to CSOPESY Group 3's Lite OS! \n";
        std::cout << "\n _____ _____ _____ _____ _____ _____ __ __    _____     _       _    _____ _____ \n";
        std::cout << "|     |   __|     |  _  |   __|   __|  |  |  |   __|___| |_ ___|_|  |     |   __|\n";
        std::cout << "|   --|__   |  |  |   __|   __|__   |_   _|  |  |  |___|  _|  _| |  |  |  |__   |\n";
        std::cout << "|_____|_____|_____|__|  |_____|_____| |_|    |_____|   |_| |_| |_|  |_____|_____|\n";
        std::cout << "\n------------------------------------- \n";
    }
}

void MainMenu::process() {

    // Setup Console Handle
    HANDLE consoleHandle = ConsoleManager::getInst()->getConHandle();

    bool validInput = false;

    while (!validInput) {
        // Enter Commands
        std::string command;
        std::cout << "\nEnter command (\"help\" for list of commands ): ";
        std::getline(std::cin, command);

        if (command == "help") {
            help();
        }

        if (command == "clear") {
            system("cls");
        }

        if (command == "exit") {
            ConsoleManager::getInst()->exitApp();
        }

        if (command == "marquee") {
            break;

        }

        if (command == "screen") {
            break;
        }

        if (command == "schedule") {
            break;
        }

    }


}