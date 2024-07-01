#include "ProcessScreen.h"
#include <iostream>
#include <cstdlib>



ProcessScreen* ProcessScreen::singletonInstance = nullptr;
ProcessScreen* ProcessScreen::getInstance(){

  return singletonInstance;

};

void ProcessScreen::initialize(){

  singletonInstance = new ProcessScreen();
  singletonInstance->isRunning = false;

};


void ProcessScreen::runScreen() {

  singletonInstance->isRunning = true;

  system("clear");
  std::cout << "Process name: " << screenProcess.getProcessName() << "\n" << std::endl;
  std::cout << "Process ID: " << screenProcess.getProcessID() << "\n" << std::endl;

  while(isRunning) {
    
    std::string input;
    std::cout << "Enter a command: " << std::endl;
    std::cin >> input;
    processUserInput(input);

  }

}

void ProcessScreen::processUserInput(std::string input){

  if(input == "exit") {

    singletonInstance->isRunning = false;
    //return to main menu

  }
  else if (input == "process-smi") {

    screenProcess.printProcessProgress();

  }
  else {

    std::cout << "Invalid input. Only valid inputs are exit or process-smi\n" << std::endl;

  }

}; 
