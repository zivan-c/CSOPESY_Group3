#include "Process.h"
#include <string>
#include <random>
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cmath>

int Process::processCount = 0;
int Process::processIDCount = 0;

Process::Process(std::string name, int instructionsLowerBound, int instructionsHigherBound, 
                 int processMemoryLower, int processMemoryHigher){

  //Randomizer to set the amount of instructions for the process
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(instructionsLowerBound, instructionsHigherBound);

  this->processName = name;
  this->processID = processIDCount;

  processIDCount++;

  this->totalInstructions = dis(gen);
  this->remainingInstructions = this->totalInstructions;
  this->processState = Process::ProcessState::READY;

  //for memory
  int base = 2; 
  std::uniform_int_distribution<> disMemory(processMemoryLower, processMemoryHigher);
  int exponent = disMemory(gen);
  this->memoryAmount = static_cast<int>(pow(base, exponent)); 
  

};

//Instruction execution, with an update for the latest instruction time
void Process::executeInstruction() {

  if(this != nullptr){
    if (this->processState == PROCESSING){
      if (this->remainingInstructions == 0){
        this->processState = Process::ProcessState::FINISHED;
      } else {
          remainingInstructions--;
          this->instructionDateAndTime = getDateAndTime(); 
      }
    } 
  }
};


int Process::getRemainingInstructions() {

  if(this != nullptr){
    return remainingInstructions;
  }else{
    return -1;
  }
};

int Process::getTotalInstructions() {

  if(this != nullptr){
   return totalInstructions;
  }else{
    return -1;
  }

};

void Process::setProcessState(Process::ProcessState state) {

  if(this != nullptr){
    this->processState = state;
  }

};

Process::ProcessState Process::getProcessState() {

  if(this != nullptr){
    return this->processState;
  }else{
    return PROCESSING;
  }

};

std::string Process::getProcessName(){

  if(this != nullptr){
    return processName;
  }else{
    return nullptr;
  }

};

int Process::getProcessID(){

  if(this != nullptr){
    return processID;
  }else{
    return -1;
  }


};

void Process::setCoreID(int coreID) {

  this->coreID = coreID;

};

int Process::getCoreID() {

  if(this != nullptr){
    return this->coreID;
  }else{
    return -1;
  }

};


//Prints the progress of itself for screen-s and screen-r commands
void Process::printProcessProgress() {

  std::cout << "Process: " << this->processName << "\n" << std::endl;
  std::cout << "ID: " << this->processID << "\n\n" << std::endl;

  if (remainingInstructions == 0) {

    std::cout << "Process finished!" << "\n" << std::endl;

  } else {

    std::cout << "Current Instruction Line: " << this->remainingInstructions << "\n" << std::endl;
    std::cout << "Lines of code: " << this->totalInstructions << "\n" << std::endl;

  }

};

//Gets the date and time, should change depending on Windows/Linux implementation
//localtime_r for Linux, and localtime_s for Windows
std::string Process::getDateAndTime(){

  auto now = std::chrono::system_clock::now();
  std::time_t calendarTime = std::chrono::system_clock::to_time_t(now);

  std::tm local_tm;
  localtime_r(&calendarTime, &local_tm); 

  std::ostringstream oss;
  oss << std::put_time(&local_tm, "(%m/%d/%Y %H:%M:%S)"); 

  return oss.str();

};



std::string Process::getInstructionTime(){

  return this->instructionDateAndTime;

};




