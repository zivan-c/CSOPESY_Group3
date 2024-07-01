#pragma once
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <thread>
#include "Scheduler.h"


class CPUCore{

public:

  CPUCore(int id, float executionDelay);
  CPUCore(int id, float executionDelay, int quantumCycles);

  bool isRunning;
  bool isAvailable;

  void runCore();
  std::string getDateandTime();
  void executeProcess();
  std::shared_ptr<Process> getProcessinCPUCore();
  int getCoreID();

  void getProcessFromReadyQueue(std::vector<std::shared_ptr<Process>>& readyQueue);
  void attachProcesstoCPUCore(std::shared_ptr<Process> process);
  void returnProcesstoReadyQueue(std::vector<std::shared_ptr<Process>>& readyQueue);
  void removeProcessinCPUCore();
  void addToFinishedList(std::vector<std::shared_ptr<Process>>& finishedQueue);

private:

  std::thread coreThread;
  ~CPUCore() = default;

  int cpuCoreID;
  int quantumCycles;
  float executionDelay;
  std::shared_ptr<Process> processInCPUCore;

  void normalCPUBehavior();
  void RRCPUBehavior();

};
