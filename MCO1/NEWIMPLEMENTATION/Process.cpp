#include "Process.h"
#include <ctime>
#include <random>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <iostream>

//Initializes the process number and ID count for the process creations 
//in the SchedulerManager
int Process::processCount = 0;
int Process::processIDCount = 0;

//Constructor for a process
Process::Process(std::string processName, int instructionsLowerBound, int instructionsHigherBound,
          int memoryLowerBound, int memoryHigherBound, int pCount){


  //Defining Instruction Amount
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(instructionsLowerBound, instructionsHigherBound);
  this->totalInstructions = dis(gen);
  this->remainingInstructions = totalInstructions;

  this->processName = processName;

  //Sets its ID to the processIDCount and increments it for the next
  //created process
  this->processID = processIDCount;
  processIDCount++;

  //Sets the process to ready when pushed to the ready queue
  this->processState = Process::READY;

  //Defining Memory Amount
  int base = 2; 
  std::uniform_int_distribution<> disMemory(memoryLowerBound, memoryHigherBound);
  int exponent = disMemory(gen);
  this->processMemory = static_cast<int>(pow(base, exponent)); 

  this->pageCount = pCount;
  isFinished = 0;

};

//Executes an instruction
void Process::executeInstruction(){

  remainingInstructions--;
  instructionTime = getTime();

  if(remainingInstructions == 0){
    processState = ProcessState::FINISHED;
    isFinished = 1;
  }

};

//Gets the time for the latest instruction execution
std::string Process::getTime(){


  auto now = std::chrono::system_clock::now();
  std::time_t calendarTime = std::chrono::system_clock::to_time_t(now);

  std::tm local_tm;
  localtime_r(&calendarTime, &local_tm);

  std::ostringstream oss;
  oss << std::put_time(&local_tm, "(%m/%d/%Y %H:%M:%S)");

  return oss.str();

}

//Prints the final process completion
void Process::printFinal(){

  std::cout << processName << " " << instructionTime << " FINISHED " << totalInstructions
  << "/" << totalInstructions <<  std::endl;

};


//Method to write to the external text file
void Process::writeToBackingStore(std::ostream& out) const {
 
  out << processID << " " << processName << " " 
  << remainingInstructions << " " << totalInstructions << " " 
  << processMemory << std::endl;

};

//Writes to an external file to store its information
//for a backing store emulation
void Process::serializeToFile(const std::string& filename) {

  std::ifstream infile(filename);
  std::stringstream buffer;
  buffer << infile.rdbuf();
  std::string fileContents = buffer.str();
  infile.close();

  std::ofstream outfile(filename, std::ios::trunc);
  std::istringstream iss(fileContents);
  std::string line;
  bool updated = false;

  while (std::getline(iss, line)) {
      std::istringstream lineStream(line);
      int id;
      lineStream >> id;
      if (id == processID) {
          writeToBackingStore(outfile);
          updated = true;
      } else {
          outfile << line << std::endl;
      }
  }

  if (!updated) {
      writeToBackingStore(outfile);
  }

  outfile.close();

}
