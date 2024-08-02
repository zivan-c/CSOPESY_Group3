#include "SchedulerManager.h"
#include "ReadyAndFinished.h"
#include "CPUCore.h"
#include "FlatMemoryAllocator.h"
#include "PagingAllocator.h"

#include <string>
#include <iostream> 
#include <random>

void commandCheck(std::string input, int pageCount);

int main(){
  
  bool running = 1;
  int cpuCores = 32;
  int quantumCycles = 5;
  int lowerInstructionsBound = 100;
  int higherInstructionsBound = 100;
  float executionDelay = 0.01;
  float creationDelay = 0.25;
  
  //for Week8 Homework
  int overallMemory = 32768;
  int processMemoryLower = 10;
  int processMemoryHigher = 14;
  int processMemory;
  int lowerPageCount = 1;
  int higherPageCount = 1;
  int pageCount;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(lowerPageCount, higherPageCount);
  pageCount = dis(gen);


  int base = 2; 
  std::uniform_int_distribution<> disMemory(processMemoryLower, processMemoryHigher);
  int exponent = disMemory(gen);
  processMemory = static_cast<int>(pow(base, exponent)); 

  int pageSize = processMemory/pageCount;


  if(pageCount == 1){

    FlatMemoryAllocator::initialize(overallMemory);
    ReadyAndFinished::initialize();
    SchedulerManager::initialize(cpuCores, quantumCycles, creationDelay, 
                          executionDelay, lowerInstructionsBound, higherInstructionsBound, 
                           processMemoryLower, processMemoryHigher, pageCount);


  }else{

    PagingAllocator::initialize(overallMemory, pageSize, pageCount);
    ReadyAndFinished::initialize();
    SchedulerManager::initialize(cpuCores, quantumCycles, creationDelay, 
                          executionDelay, lowerInstructionsBound, higherInstructionsBound, 
                           processMemoryLower, processMemoryHigher, pageCount);
  }


  while(running){

    std::cout << "Commands:" << std::endl;
    std::cout << "scheduler-test, scheduler-stop, screen-ls" << std::endl;
    std::cout << "report-util, exit, process-smi, vmstat" << std::endl;
    std::cout << "Enter command: ";
    std::string input;
    std::cin >> input;
    if(input == "exit"){
      running = 0;
    }else{
      commandCheck(input, pageCount);
    }

  }      
  
};

void commandCheck(std::string input, int pCount){



  if(input == "scheduler-test"){

    SchedulerManager::getInstance()->createProcesses();

  }
  else if(input == "scheduler-stop"){

    SchedulerManager::isCreatingProcesses = 0;

  }
  else if(input == "screen-ls"){

    CPUCore::isPrinting = 1;
    SchedulerManager::getInstance()->screenLS();
    CPUCore::isPrinting = 0;

  }
  else if(input == "report-util"){

    CPUCore::isPrinting = 1;
    SchedulerManager::getInstance()->reportUtil();
    CPUCore::isPrinting = 0;
  
  }
  else if(input == "process-smi"){

    CPUCore::isPrinting = 1;
    if(pCount > 1){
      PagingAllocator::getInstance()->printProcessesInMemory();
    }else{
      FlatMemoryAllocator::getInstance()->printProcessesInMemory();
    }
    CPUCore::isPrinting = 0;

  }
  else if(input == "vmstat"){

    CPUCore::isPrinting = 1;
    if(pCount > 1){
      PagingAllocator::getInstance()->vmStat();
    }else{
      FlatMemoryAllocator::getInstance()->vmStat();
    }
    CPUCore::isPrinting = 0;
  }

  else{

    std::cout << "Invalid input" << std::endl;

  }

};
