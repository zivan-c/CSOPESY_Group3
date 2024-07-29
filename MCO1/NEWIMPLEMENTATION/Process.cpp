#include "Process.h"
#include <ctime>
#include <random>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <iostream>

int Process::processCount = 0;
int Process::processIDCount = 0;

Process::Process(std::string processName, int instructionsLowerBound, int instructionsHigherBound,
          int memoryLowerBound, int memoryHigherBound, int minimumPaging, int maximumPaging){


  //Defining Instruction Amount
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(instructionsLowerBound, instructionsHigherBound);
  this->totalInstructions = dis(gen);
  this->remainingInstructions = totalInstructions;

  this->processName = processName;

  this->processID = processIDCount;
  processIDCount++;

  this->processState = Process::READY;

  //Defining Memory Amount
  int base = 2; 
  std::uniform_int_distribution<> disMemory(memoryLowerBound, memoryHigherBound);
  int exponent = disMemory(gen);
  this->processMemory = static_cast<int>(pow(base, exponent)); 

  //Define Pages (If needed)
  if(!(minimumPaging == 1) && (maximumPaging == 1)){ 

    //declare pages here
    //
  }else{

  }
  
  isFinished = 0;

};

void Process::executeInstruction(){

  remainingInstructions--;
  instructionTime = getTime();

  if(remainingInstructions == 0){
    processState = ProcessState::FINISHED;
    isFinished = 1;
  }

};


std::string Process::getTime(){


  auto now = std::chrono::system_clock::now();
  std::time_t calendarTime = std::chrono::system_clock::to_time_t(now);

  std::tm local_tm;
  localtime_r(&calendarTime, &local_tm);

  std::ostringstream oss;
  oss << std::put_time(&local_tm, "(%m/%d/%Y %H:%M:%S)");

  return oss.str();

}

void Process::printFinal(){

  std::cout << processName << " " << instructionTime << " FINISHED " << totalInstructions
  << "/" << totalInstructions <<  std::endl;

};

void Process::writeToBackingStore(std::ostream& out) const {
 
  out << processID << " " << processName << " " 
  << remainingInstructions << " " << totalInstructions << " " 
  << processMemory << std::endl;

};

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
