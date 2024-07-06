#include "ProcessScreen.h"
#include <iostream>
#include <cstdlib> 
#include <thread>


//Overrides the virtual function
//Should only be run once a process has been attached
void ProcessScreen::runConsole() {

    std::thread processScreenThread;
    processScreenThread = std::thread([this]() {
        this->isRunning = true;
        system("clear"); //change for windows version
        std::cout << "Process name: " << screenProcess->getProcessName() << "\n" << std::endl;
        std::cout << "Process ID: " << screenProcess->getProcessID() << "\n" << std::endl;

        //While running for process-smi and exit commands
        while (isRunning) {

            std::string input;
            std::cout << "Enter a command: " << std::endl;
            std::cin >> input;
            processUserInput(input);

        }

        });

};

//Processes user input
void ProcessScreen::processUserInput(std::string input) {

    if (input == "exit") {

        this->isRunning = false;
        //insert code for returning to the main menu

    }
    else if (input == "process-smi") {

        screenProcess->setProcessState(Process::WAITING);
        screenProcess->printProcessProgress();
        screenProcess->setProcessState(Process::PROCESSING);

    }
    else {

        std::cout << "Invalid input. Only valid inputs are exit or process-smi\n" << std::endl;

    }

};


//Attaches process to the object
void ProcessScreen::attachProcessToScreen(std::shared_ptr<Process> process) {

    this->screenProcess = process;

};