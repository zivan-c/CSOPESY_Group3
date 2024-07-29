#include "ProcessScreen.h"
#include "CPUScheduler.h"
#include "FirstFit.h"

#include <string>
#include <iostream> 

void commandCheck(std::string input);

int main(){
  
  bool running = 1;
  int cpuCores = 16;
  int quantumCycles = 4;
  int lowerInstructionsBound = 100;
  int higherInstructionsBound = 100;
  float executionDelay = 0.01;
  float creationDelay = 0.25;
  int preemptive = 1;
  
  //for Week8 Homework
  int overallMemory = 16384;
  int processMemoryLower = 4;
  int processMemoryHigher = 12;

  CPUScheduler::SchedulerAlgorithm schedulerAlgorithm = CPUScheduler::RR;


  ReadyQueue::initialize();
  FirstFit::initialize(overallMemory);
  CPUScheduler::initialize(cpuCores, schedulerAlgorithm, executionDelay, quantumCycles, preemptive,
                          creationDelay, lowerInstructionsBound, higherInstructionsBound, 
                           processMemoryLower, processMemoryHigher);

  

  while(running){

    std::cout << "Commands:" << std::endl;
    std::cout << "scheduler-test, scheduler-stop, screen-ls" << std::endl;
    std::cout << "report-util, createprocess, exit" << std::endl;
    std::cout << "Enter command: ";
    std::string input;
    std::cin >> input;
    if(input == "exit"){
      running = 0;
    }else{
      commandCheck(input);
    }

  }      
  
};

void commandCheck(std::string input){



  if(input == "scheduler-test"){

    CPUScheduler::getInstance()->startScheduler();

  }
  else if(input == "scheduler-stop"){

    CPUScheduler::getInstance()->stopScheduler();

  }
  else if(input == "screen-ls"){

    CPUScheduler::getInstance()->printReport();

  }
  else if(input == "report-util"){

    CPUScheduler::getInstance()->createReportFile();
  
  }
  else if(input == "createprocess"){
  
    std::string processName;
    std::cin >> processName;
    CPUScheduler::getInstance()->createProcess(processName);

  }
  else if(input == "process-smi"){

    FirstFit::getInstance()->isPrinting = true;
    CPUScheduler::getInstance()->processSMI();
    FirstFit::getInstance()->isPrinting = false;

  }
  else{

    std::cout << "Invalid input" << std::endl;

  }

};
