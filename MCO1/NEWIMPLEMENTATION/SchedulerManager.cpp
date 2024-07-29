#include "SchedulerManager.h"
#include "ReadyAndFinished.h"
#include "FlatMemoryAllocator.h"
#include "CPUCore.h"
#include "Process.h"

#include <iostream>
#include <fstream>


int SchedulerManager::isCreatingProcesses = 0;

SchedulerManager *SchedulerManager::singletonInstance = nullptr;
SchedulerManager *SchedulerManager::getInstance() { return singletonInstance; };

void SchedulerManager::initialize(int cpuCount, int qCount, float cDelay, float eDelay,
                    int iLowerBound, int iHigherBound, int mLowerBound,
                    int mHigherBound, int pLowerBound, int pHigherBound){

  CPUCore::idleTicks = 0;
  CPUCore::activeTicks = 0;
  Process::processIDCount = 0;  
  Process::processCount = 0;
  SchedulerManager::isCreatingProcesses = 0;

  singletonInstance = new SchedulerManager();
  singletonInstance->CPUCoreCount = cpuCount;
  singletonInstance->quantumCycleCount = qCount;
  singletonInstance->creationDelay = cDelay;
  singletonInstance->executionDelay = eDelay;
  singletonInstance->instructionsLowerBound = iLowerBound;
  singletonInstance->instructionsHigherBound = iHigherBound;
  singletonInstance->memoryLowerBound = mLowerBound;
  singletonInstance->memoryHigherBound = mHigherBound;
  singletonInstance->pageLowerBound = pLowerBound;
  singletonInstance->pageHigherBound = pHigherBound;

  singletonInstance->setupCores();

  for (auto& core : singletonInstance->coreList) {
    core->run();
  }

};

void SchedulerManager::setupCores(){

  int isFlatMemory;
  if((pageLowerBound == 1) && (pageHigherBound == 1)){
    isFlatMemory = 1;
  }else{
    isFlatMemory = 0;
  }

  for(size_t i = 0; i < CPUCoreCount; i++){

    std::shared_ptr<CPUCore> cpuCore = std::make_shared<CPUCore>
      (i + 1, executionDelay, quantumCycleCount, isFlatMemory);

    coreList.push_back(cpuCore);

  }

}

std::string SchedulerManager::generateProcessName(){

  std::string name = "Process_" + std::to_string(Process::processCount);
  Process::processCount++;
  return name;

}


void SchedulerManager::createProcesses(){

  isCreatingProcesses = 1;

  std::thread processCreationThread;
  processCreationThread = std::thread([this]() {

    while(isCreatingProcesses){

      std::string processName = generateProcessName();

      std::shared_ptr<Process> newProcess = std::make_shared<Process>(processName,
      instructionsLowerBound, instructionsHigherBound, memoryLowerBound, 
      memoryHigherBound, pageLowerBound, pageHigherBound);

      ReadyAndFinished::getInstance()->pushProcessToReady(newProcess);
      newProcess.reset();

      std::chrono::duration<float, std::milli> delayDuration(creationDelay * 1000); 
      std::this_thread::sleep_for(delayDuration);


    }


  });
  processCreationThread.detach();

};


void SchedulerManager::screenLS(){

  int coresUsed = 0;
  int coresAvailable = 0;
  std::vector<std::shared_ptr<CPUCore> > runningList;

  for (auto& i : this->coreList) {
    if (!(i->isFree)) {
      coresUsed++;
      runningList.push_back(i);
    } else {
      coresAvailable++;
    }
  }

  double percentage = ((double)coresUsed / (double)CPUCoreCount) * 100;

  std::cout << "\nCPU utilization: " << percentage << "%\n" << std::endl;
  std::cout << "Cores used: " << coresUsed << std::endl;
  std::cout << "Cores available: " << coresAvailable << "\n" << std::endl;
  std::cout << "----------------------------------------" << "\n"
              << std::endl;

  std::cout << "Running processes:" << std::endl;

  for (auto i : this->coreList) {

      auto process = i->processInCore;
      if (process != nullptr) {  // Check if the process is not a null pointer
          
          std::string processName = process->processName;
          std::string processInstructionTime = process->instructionTime;
          int coreID = i->coreID;
          int remainingInstructions = process->remainingInstructions;
          int totalInstructions = process->totalInstructions;
          int processMemory = process->processMemory;

          std::cout << processName << " " << 
                    processInstructionTime << " Core: " <<
                    coreID << " " << 
                    remainingInstructions << "/" <<
                    totalInstructions << " Memory: " << processMemory << std::endl;
    }
  }
  std::cout << "\nFinished processes:" << std::endl;
  for(auto& i : ReadyAndFinished::getInstance()->finishedList){

    i->printFinal();

  }
  

};


void SchedulerManager::reportUtil(){

  std::ofstream outputFile; 
  outputFile.open("csopesy-log.txt");

  if(!outputFile){
  
    std::cout << "File could not be opened." << std::endl;

  }else{

    int coresUsed = 0;
    int coresAvailable = 0;
    std::vector<std::shared_ptr<CPUCore> > runningList;

    for (auto& i : this->coreList) {
      if (!(i->isFree)) {
        coresUsed++;
        runningList.push_back(i);
      } else {
        coresAvailable++;
      }
    }

    double percentage = ((double)coresUsed / (double)CPUCoreCount) * 100;

    outputFile << "CPU utilization: " << percentage << "%\n" << std::endl;
    outputFile << "Cores used: " << coresUsed << std::endl;
    outputFile << "Cores available: " << coresAvailable << "\n" << std::endl;
    outputFile << "----------------------------------------" << "\n"
                << std::endl;

    outputFile << "Running processes:" << std::endl;

    for (auto i : this->coreList) {

        auto process = i->processInCore;
        if (process != nullptr) {  // Check if the process is not a null pointer
            
            std::string processName = process->processName;
            std::string processInstructionTime = process->instructionTime;
            int coreID = i->coreID;
            int remainingInstructions = process->remainingInstructions;
            int totalInstructions = process->totalInstructions;
            int processMemory = process->processMemory;

            outputFile << processName << " " << 
                      processInstructionTime << " Core: " <<
                      coreID << " " << 
                      remainingInstructions << "/" <<
                      totalInstructions << " Memory: " << processMemory << std::endl;
      }
    }
    outputFile << "\nFinished processes:" << std::endl;

    for(auto& i : ReadyAndFinished::getInstance()->finishedList){

      outputFile << i->processName << " " << i->instructionTime << " FINISHED " << i->totalInstructions
  << "/" << i->totalInstructions << std::endl;
      
    }

    outputFile << "----------------------------------------" << "\n"
                << std::endl;

  }
};


