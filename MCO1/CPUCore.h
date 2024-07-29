#pragma once
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <thread>
#include "Scheduler.h"
#include <mutex>
#include <semaphore>

class CPUCore{

public:

  CPUCore(int id, float executionDelay);
  CPUCore(int id, float executionDelay, int quantumCycles);
  ~CPUCore() = default;

  bool isRunning;

  int isCoreFree();
  void runCore();
  void executeProcess();
  std::shared_ptr<Process> getProcessinCPUCore();
  int getCoreID();

  void getProcessFromReadyQueue();
  void returnProcesstoReadyQueue();
  void addToFinishedList();
  void removeProcessinCPUCore();
  static std::counting_semaphore<1> sem;  
  

private:

  std::thread coreThread;

  //for synchronization

  bool isAvailable;

  int cpuCoreID;
  int quantumCycles;
  float executionDelay;
  std::shared_ptr<Process> processInCPUCore;

  void normalCPUBehavior();
  void RRCPUBehavior();


  //for memory
  void* processPointer; 

};
