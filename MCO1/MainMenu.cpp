// OLD VERSION //
 #pragma once
#include "MainMenu.h"
#include <Windows.h>
#include "Console.h"
#include "ConsoleManager.h"
#include "OSConfig.h"
#include "Scheduler.h"
#include <string.h>
#include "Process.h"
#include "marqueeWorkerThread.h"  // Include the header for marquee functionality

MainMenu::MainMenu() : Console(MAIN) {
}

MainMenu::~MainMenu() {
    // Cleanup code if necessary
}

// Define the activate method
void MainMenu::activate() {
    this->isNew = true;
}

// Define the display method
void MainMenu::display() {
}

void MainMenu::setConfig()
{
    OSConfig::initialize();
    OSConfig::readConfig();
}

void MainMenu::help() {
    std::cout << "\nCommand list: \n";
    std::cout << "help              Shows all commands\n";
    std::cout << "marquee           Open the Marquee Console\n";
    std::cout << "initialize        Initialize the OS's processor\n";
    std::cout << "scheduler-test    Generate test processes\n";
    std::cout << "scheduler-stop    Stop process scheduler\n";
    std::cout << "screen            Manage screens\n";
    std::cout << "clear             Clear the screen\n";
    std::cout << "exit              Exit OS\n";
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
    // Print Header
    printHeader();

    // Display Instructions
    help();

    //Valid Input
    bool validInput = false;
    bool initialized = false;

    // Setup Console Handle
    HANDLE consoleHandle = ConsoleManager::getInst()->getConHandle();

    while (!validInput) {
        // Enter Commands
        std::string command;
        std::cout << "\nEnter command (\"help\" for list of commands ): ";
        std::getline(std::cin, command);

        if (command == "exit") {
            ConsoleManager::getInst()->exitApp();
            std::cout << "Exiting the OS..." << std::endl;
            exit(0);
        }

        if (command == "initialize") {
            OSConfig::initialize();
            OSConfig::readConfig();
            initialized = true;
        }

        if (initialized == true) {
            if (command == "clear") {
                system("cls");
            }

            if (command == "help") {
                help();
            }

            if (command == "marquee") {
                // Call the marquee function
                // marquee();
            }

            if (command == "schedule") {
                // switch to schedule console
            }

            if (command == "scheduler-test") {
            }

            if (command == "scheduler-stop") {
            }

            if (command.find("screen") != std::string::npos) {
                if (command.find(" -ls") != std::string::npos) {
                }

                else if (command.find(" -r") != std::string::npos) {
                    std::string exsProcess = command.erase(0, 8);
                }

                else if (command.find(" -s") != std::string::npos) {
                    std::string newProcess = command.erase(0, 8);
                }
            }
        }
        
        else{
            std::cout << "Operating System not initialized. Enter \"initialize\"...\n";
        }
        
    }
}