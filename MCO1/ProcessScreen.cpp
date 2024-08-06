#include "ProcessScreen.h"
#include <iostream>

/*
void run();
void static initialize();
static ProcessScreen* getInstance();
private:

static ProcessScreen* singletonInstance;
void inputProcess();
*/

ProcessScreen* ProcessScreen::singletonInstance = nullptr;
ProcessScreen* ProcessScreen::getInstance() { return singletonInstance; };

void ProcessScreen::initialize() {
    singletonInstance = new ProcessScreen();
};

void ProcessScreen::run(std::shared_ptr<Process> process) {

    isRunning = 1;
    system("cls");
    while (isRunning) {
        
        std::cout << "Process: " << process->processName << std::endl;
        std::cout << "ID: \n" << process->processID << std::endl;
        std::cout << "Commands (process-smi, exit)" << std::endl;

        std::string input;
        std::cin >> input;
        if (input == "process-smi") {

            if (process->processState == Process::FINISHED) {

                std::cout << "Finished!" << std::endl;

            }
            else {

                std::cout << "Remaining Instructions: " << process->remainingInstructions << std::endl;
                std::cout << "Total Instructions: " << process->totalInstructions << std::endl;

            }



        }
        else if (input == "exit") {
            isRunning = 0;
        }
        else {
            std::cout << "Invalid input" << std::endl;
        }

    }


};


void ProcessScreen::inputProcess(std::string input, std::shared_ptr<Process> process) {


    if (input == "process-smi") {

    }
    else if (input == "exit") {

        isRunning = 0;

    }
    else {

        std::cout << "Invalid input" << std::endl;

    }

};