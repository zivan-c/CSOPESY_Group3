#include "ProcessScreen.h"
#include <iostream>
#include <cstdlib> //linux, change to windows if needed
#include <thread>


void ProcessScreen::runConsole(){

    std::thread processScreenThread;
    processScreenThread = std::thread([this](){
    this->isRunning = true;
    system("clear");
    std::cout << "Process name: " << screenProcess->getProcessName() << "\n" << std::endl;
    std::cout << "Process ID: " << screenProcess->getProcessID() << "\n" << std::endl;

    while(isRunning) {
    
      std::string input;
      std::cout << "Enter a command: " << std::endl;
      std::cin >> input;
      processUserInput(input);

    }

  });

};

void ProcessScreen::processUserInput(std::string input){

  if(input == "exit") {

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


void ProcessScreen::attachProcessToScreen(std::shared_ptr<Process> process){

  this->screenProcess = process;

};


