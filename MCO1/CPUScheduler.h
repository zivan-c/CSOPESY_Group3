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
#include <mutex>


class CPUScheduler {


public:
    CPUScheduler();
    virtual ~CPUScheduler();

    enum SchedulerAlgorithm {
        FCFS,
        RR,
        SJF

    };

    static CPUScheduler* getInstance();

    static void initialize(int cpuCores, SchedulerAlgorithm schedulerAlgorithm, float executionDelay,
        int quantumCycles, int preemptive, int creationDelay,
        int instructionsLowerBound, int instructionsHigherBound);

    void setupCPUS();
    void setupScheduler(); //include parameters for algorithm, quantum cycles, etc 
    void createDynamicProcesses(); //for scheduler-start, just the creation of new processes
    void createProcess(std::string name);
    void startScheduler();
    void stopScheduler();

    void printReport();
    void createReportFile();

    std::string getNewProcessName();



    //creating mutex functions for readyQueue, all functions related to it are accessed here

    std::shared_ptr<Process> removeProcessFromReadyQueue();
    void addProcessToReadyQueue(std::shared_ptr<Process> process);
    int returnLowestRemainingInstructions();
    bool isReadyQueueAvailable();

    void sortReadyQueue();


    void addProcessToFinishedProcesses(std::shared_ptr<Process> process);

    //to 
    //static void returntoReadyQueue(std::shared_ptr<Process> process);
    //static void placeInFinishedProcesses(std::shared_ptr<Process> process);

  //When initialized, set the CPU core numbers and initialize them with their respective schedulers and ready queue
  //The CPU Scheduler should have the waiting queue,

    std::vector <std::shared_ptr<CPUCore> > cpuCores;

    int cpuCoresAmount;
    SchedulerAlgorithm scheduler;
    int quantumCycles;
    int preemptive;
    float creationDelay;
    int instructionsLowerBound;
    int instructionsHigherBound;
    float executionDelay;

private:
    std::mutex queueMutex;
    bool keepGenerating;
    std::shared_ptr<Scheduler> CPUSchedulerAlgorithm;
    static CPUScheduler* singletonInstance;
    std::vector <std::shared_ptr<Process> > readyQueue;
    std::vector <std::shared_ptr<Process> > finishedProcesses;

};