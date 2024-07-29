#include "FCFSScheduler.h"
#include "RRScheduler.h"
#include "PreemptiveSJF.h"
#include "NonPreemptiveSJF.h"
#include "Scheduler.h"
#include "Process.h"
#include "CPUCore.h"
#include "Console.h"
#include "ReadyQueue.h"

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <mutex>


class CPUScheduler{


public:

  enum SchedulerAlgorithm{
    FCFS,
    RR,
    SJF

  };

  static CPUScheduler* getInstance();
  
  static void initialize(int cpuCores, SchedulerAlgorithm schedulerAlgorithm, float executionDelay,
                          int quantumCycles, int preemptive, float creationDelay,
                          int instructionsLowerBound, int instructionsHigherBound, 
                         int processMemoryLower, int processMemoryHigher);

  void setupCPUS();
  void setupScheduler(); //include parameters for algorithm, quantum cycles, etc 
  void createDynamicProcesses(); //for scheduler-start, just the creation of new processes
  void createProcess(std::string name);
  void startScheduler();
  void stopScheduler();
  std::shared_ptr<Process> getProcessPointer(std::string process);

  void printReport();
  void createReportFile();

  std::string getNewProcessName();



  //creating mutex functions for readyQueue, all functions related to it are accessed here



  void addProcessToFinishedProcesses(std::shared_ptr<Process> process);


  //for memory progress printing
  void printMemoryProgress();

  //for processSMI checking();
  void processSMI();

  //to 
  //static void returntoReadyQueue(std::shared_ptr<Process> process);
  //static void placeInFinishedProcesses(std::shared_ptr<Process> process);

//When initialized, set the CPU core numbers and initialize them with their respective schedulers and ready queue
//The CPU Scheduler should have the waiting queue,

  std::vector <std::shared_ptr<CPUCore> > cpuCores;

  int cpuCoresAmount;
  int quantumCycles;
  int preemptive;
  float creationDelay;
  int instructionsLowerBound;
  int instructionsHigherBound;
  float executionDelay;
  int overallMemory;
  int processMemoryLower;
  int processMemoryHigher;
  int quantumCycleAmount;

private:
  std::mutex queueMutex;
  bool keepGenerating;
  SchedulerAlgorithm scheduler;
  std::shared_ptr<Scheduler> CPUSchedulerAlgorithm;
  static CPUScheduler* singletonInstance;
  std::vector <std::shared_ptr<Process> > finishedProcesses;
  
};
