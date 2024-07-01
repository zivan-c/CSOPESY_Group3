#pragma once
#include <string>
#include <iostream>
#include <cstdlib>
#include <random>
#include "Console.h"


class Process{

public:

  enum ProcessState {

      READY,
      PROCESSING,
      WAITING,
      FINISHED

  };
  static int processIDCount;
  static int processCount;

  Process(std::string name, int instructionsLowerBound, int instructionsHigherBound);
  void executeInstruction();
  int getRemainingInstructions();
  int getTotalInstructions();
  void setProcessState(ProcessState state);
  ProcessState getProcessState();
  std::string getProcessName();
  int getProcessID();
  int getCoreID();
  void setCoreID(int coreID);
  void printProcessProgress();
  std::string getDateAndTime();


private:
  std::string processName;
  int processID;
  int coreID;
  int totalInstructions;
  int remainingInstructions;
  ProcessState processState;
  std::string instructionDateAndTime; 

};


