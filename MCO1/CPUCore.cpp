#include "CPUCore.h"
#include "CPUScheduler.h"
#include "Process.h"
#include <chrono>
#include <thread>
#include <ctime>


CPUCore::CPUCore(int id, float executionDelay) {

  this->cpuCoreID = id;
  this->executionDelay = executionDelay;
  this->quantumCycles = 0;

};

CPUCore::CPUCore(int id, float executionDelay, int quantumCycles){

  this->cpuCoreID = id;
  this->executionDelay = executionDelay;
  this->quantumCycles = quantumCycles;

};

void CPUCore::executeProcess(){
  this->processInCPUCore->executeInstruction();
};

std::shared_ptr<Process> CPUCore::getProcessinCPUCore(){

  return processInCPUCore;

};


void CPUCore::runCore(){


  coreThread = std::thread([this](){
    this->isRunning = true;

    while(isRunning){

      if(processInCPUCore != nullptr){

        if(quantumCycles){
          RRCPUBehavior();
        }
        else{
          normalCPUBehavior();
        }

      }
    };
  });

  coreThread.detach();

};

      


void CPUCore::normalCPUBehavior(){

  if(processInCPUCore->getProcessState() == Process::PROCESSING){
    executeProcess();
    std::chrono::duration<float, std::milli> delayDuration(executionDelay * 1000); 
    std::this_thread::sleep_for(delayDuration);

  }else{
      
    addToFinishedList(CPUScheduler::getInstance()->finishedProcesses);

  }   
};

void CPUCore::RRCPUBehavior(){

  for(int i = 0; i < quantumCycles; i++){

    if(processInCPUCore->getProcessState() == Process::PROCESSING){

      executeProcess();
      std::chrono::duration<float, std::milli> delayDuration(executionDelay * 1000); 
      std::this_thread::sleep_for(delayDuration);

    }else{
      
      addToFinishedList(CPUScheduler::getInstance()->finishedProcesses);
      getProcessFromReadyQueue(CPUScheduler::getInstance()->readyQueue);
      break;

    }
  }

  returnProcesstoReadyQueue(CPUScheduler::getInstance()->readyQueue);
  getProcessFromReadyQueue(CPUScheduler::getInstance()->readyQueue);

  
  
};

void CPUCore::attachProcesstoCPUCore(std::shared_ptr<Process> process) {

  if(this->isAvailable){

    this->processInCPUCore = process;
    this->isAvailable = false;
    
    processInCPUCore->setCoreID(cpuCoreID);
    processInCPUCore->setProcessState(Process::PROCESSING);
  }

};


void CPUCore::getProcessFromReadyQueue(std::vector<std::shared_ptr<Process>>& readyQueue){

  if(!(readyQueue.empty())){
  
    this->processInCPUCore = readyQueue.front();
    this->isAvailable = false;
    processInCPUCore->setProcessState(Process::PROCESSING);
    readyQueue.erase(readyQueue.begin());

  }else{

    std::cout << "Ready Queue is empty!" << std::endl;

  }

};


void CPUCore::addToFinishedList(std::vector<std::shared_ptr<Process>>& finishedQueue){

    finishedQueue.push_back(processInCPUCore);
    removeProcessinCPUCore();

};

void CPUCore::returnProcesstoReadyQueue(std::vector<std::shared_ptr<Process>>& readyQueue){

    processInCPUCore->setProcessState(Process::READY);
    readyQueue.push_back(processInCPUCore);
    removeProcessinCPUCore();

};


void CPUCore::removeProcessinCPUCore(){

  processInCPUCore.reset(); //Now, the CPUCore is empty;
  this->isAvailable = true; //CPU Core is available to be used
  //
};

int CPUCore::getCoreID(){

  return this->cpuCoreID;

};

