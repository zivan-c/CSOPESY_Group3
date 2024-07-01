#pragma once
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <thread>
#include "Scheduler.h"


class CPUCore{

public:

  CPUCore(int id, int executionDelay);
  CPUCore(int id, int executionDelay, int quantumCycles);

  bool isRunning;
  bool isAvailable;

  void runCore();
  bool isProcessFinished();
  std::string getDateandTime();
  void executeProcess();
  std::shared_ptr<Process> getProcessinCPUCore();
  int getCoreID();

  void getProcessFromReadyQueue(std::vector<std::shared_ptr<Process>>& readyQueue);
  void attachProcesstoCPUCore(std::shared_ptr<Process> process);
  void returnProcesstoReadyQueue(std::vector<std::shared_ptr<Process>>& readyQueue);
  void removeProcessinCPUCore();
  void addToFinishedList(std::vector<std::shared_ptr<Process>>& finishedQueue);


  //CPU Algorithm Scheduler for each core class to be instantiated when the CPU scheduler is run 
  //
private:

  std::thread coreThread;
  ~CPUCore() = default;
  int cpuCoreID;
  int quantumCycles;
  int executionDelay;
  std::shared_ptr<Process> processInCPUCore;

  void normalCPUBehavior();
  void RRCPUBehavior();

};
