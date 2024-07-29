#pragma once
#include <string>
#include <fstream>

class Process {

public:
  
  enum ProcessState {

      READY,
      PROCESSING,
      WAITING,
      FINISHED

  };

  Process(std::string processName, int instructionsLowerBound, int instructionsHigherBound,
          int memoryLowerBound, int memoryHigherBound, int minimumPaging, int maximumPaging);
  
  static int processIDCount;
  static int processCount;

  ProcessState processState;
  std::string processName;
  std::string instructionTime;
  int processID;
  int processMemory;
  int remainingInstructions;
  int totalInstructions;
  int isFinished;


  void writeToBackingStore(std::ostream& out) const;
  void serializeToFile(const std::string& filename);
  void executeInstruction();
  void printFinal();
  std::string getTime();

private:




};
