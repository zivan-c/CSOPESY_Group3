#include "Process.h"
#include <string>
#include <random>
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>



Process::Process(std::string name, int instructionsLowerBound, int instructionsHigherBound){

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(instructionsLowerBound, instructionsHigherBound);

  this->processName = name;
  this->processID = processIDCount;

  processIDCount++;

  this->totalInstructions = dis(gen);
  this->remainingInstructions = this->totalInstructions;
  this->processState = READY;

};

void Process::executeInstruction() {
 
  if (this->processState == PROCESSING){

    if (this->remainingInstructions == 0){

      this->processState = FINISHED;
      this->instructionDateAndTime = getDateAndTime();

    } else {
        remainingInstructions--;
        this->instructionDateAndTime = getDateAndTime();
    }

  } 
    
};

int Process::getRemainingInstructions() {

  return remainingInstructions;

};

int Process::getTotalInstructions() {

  return totalInstructions;

};

void Process::setProcessState(ProcessState state) {

  this->processState = state;

};

Process::ProcessState Process::getProcessState() {

  return processState;

};

std::string Process::getProcessName(){

  return processName;

};

int Process::getProcessID(){

  return processID;

};

void Process::setCoreID(int coreID) {

  this->coreID = coreID;

};

int Process::getCoreID() {

  return this->coreID;

};

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

std::string Process::getDateAndTime(){

  auto now = std::chrono::system_clock::now();

  std::time_t calendarTime = std::chrono::system_clock::to_time_t(now);

  std::tm local_tm = *std::localtime(&calendarTime);

  std::ostringstream oss;
  oss << std::put_time(&local_tm, "(%m/%d/%Y %H:%M:%S)"); 

  std::string dateAndTime = oss.str();

  return dateAndTime;

};







