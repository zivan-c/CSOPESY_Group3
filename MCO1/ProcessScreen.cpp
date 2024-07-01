#include "ProcessScreen.h"
#include <iostream>
#include <cstdlib> //linux, change to windows if needed
#include <thread>



ProcessScreen* ProcessScreen::singletonInstance = nullptr;
ProcessScreen* ProcessScreen::getInstance(){

  return singletonInstance;

};

void ProcessScreen::initialize(){  //must run alongside initializing of scheduler

  singletonInstance = new ProcessScreen();
  singletonInstance->isRunning = false;

};


void ProcessScreen::runScreen(std::shared_ptr<Process> process) {

  std::thread processScreenThread;
  processScreenThread = std::thread([this, process](){
    singletonInstance->isRunning = true;
    attachProcess(process);
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

    singletonInstance->isRunning = false;
    //return to main menu

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


void ProcessScreen::attachProcess(std::shared_ptr<Process> process){

  this->screenProcess = process;

};


