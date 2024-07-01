#include "ProcessScreen.h"
#include "CPUScheduler.h"

#include <string>
#include <iostream> 

void commandCheck(std::string input);

int main(){
  
  bool running = 1;
  int cpuCores = 4;
  int quantumCycles = 5;
  int lowerInstructionsBound = 10;
  int higherInstructionsBound = 100;
  float executionDelay = 0.25;
  int preemptive = 0;
  CPUScheduler::SchedulerAlgorithm schedulerAlgorithm = CPUScheduler::FCFS;

  CPUScheduler::initialize(cpuCores, schedulerAlgorithm, executionDelay, quantumCycles, preemptive,
                          executionDelay, lowerInstructionsBound, higherInstructionsBound);

  ProcessScreen::initialize();
  

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
  else{

    std::cout << "Invalid input" << std::endl;

  }

};
