#include "FCFSScheduler.h"
#include "RRScheduler.h"
#include "PreemptiveSJF.h"
#include "NonPreemptiveSJF.h"
#include "Scheduler.h"
#include "Process.h"
#include "CPUCore.h"
#include "Console.h"

#include <vector>
#include <string>
#include <iostream>
#include <memory>




class CPUScheduler: public Console{


public:

  enum SchedulerAlgorithm{

    FCFS,
    RR,
    SJF

  };

  static CPUScheduler* getInstance();

  
  void initialize(int cpuCores, SchedulerAlgorithm schedulerAlgorithm, int executionDelay,
  int quantumCycles, int preemptive, int creationDelay,
  int instructionsLowerBound, int instructionsHigherBound);
  void setupCPUS();
  void setupScheduler(); //include parameters for algorithm, quantum cycles, etc 
  void createDynamicProcesses(); //for scheduler-start, just the creation of new processes
  void stopGeneratingProcesses(); //for scheduler-stop
  void createProcess(std::string name);
  void startScheduler();
  void stopScheduler();

  void printReport();
  void createReportFile();

  std::string getNewProcessName();
  static void returntoReadyQueue(Process& process);
  static void placeInFinishedProcesses(Process& process);

//When initialized, set the CPU core numbers and initialize them with their respective schedulers and ready queue
//The CPU Scheduler should have the waiting queue,

  std::vector <std::shared_ptr<Process>> readyQueue;
  std::vector <std::shared_ptr<Process>> finishedProcesses;
  std::vector <std::shared_ptr<CPUCore>> cpuCores;

  int cpuCoresAmount;
  SchedulerAlgorithm scheduler;
  int quantumCycles;
  int preemptive;
  int creationDelay;
  int instructionsLowerBound;
  int instructionsHigherBound;
  int executionDelay;

private:

  CPUScheduler();
  ~CPUScheduler();
  

  bool keepGenerating;
  Scheduler CPUSchedulerAlgorithm;

  static CPUScheduler* singletonInstance;
};
