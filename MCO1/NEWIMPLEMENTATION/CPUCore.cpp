#include "CPUCore.h"
#include "ReadyAndFinished.h"
#include "FlatMemoryAllocator.h"
#include "PagingAllocator.h"

size_t CPUCore::activeTicks = 0;
size_t CPUCore::idleTicks = 0;
int CPUCore::isPrinting = 0;

//Constructor
CPUCore::CPUCore(int coreID, float executionDelay, int quantumCycleCount, int pCount){

  this->coreID = coreID;
  this->executionDelay = executionDelay;
  this->quantumCycleCount = quantumCycleCount;
  this->pageCount = pCount;
  isRunning = 0;

};

//Starts the thread for a CPUCore
void CPUCore::run(){

  cpuCoreThread = std::thread([this](){
  this->isRunning = 1;

    while(isRunning){
        if(quantumCycleCount){
          RRBehavior();
        }
        else{
          FCFSBehavior();
        }
    };
  });

  cpuCoreThread.detach();

};


//CPU Core for a First Come First Serve Scheduler
void CPUCore::FCFSBehavior(){

  if(processInCore != nullptr){

    while(!(processInCore->isFinished)){

      processInCore->executeInstruction();
      activeTicks++;
      std::chrono::duration<float, std::milli> delayDuration(executionDelay * 1000); 
      std::this_thread::sleep_for(delayDuration);

    }

    if(isPrinting){
      std::chrono::duration<float, std::milli> printDelay(5000); 
      std::this_thread::sleep_for(printDelay);
    }

    if(!(pageCount > 1)){
      FlatMemoryAllocator::getInstance()->deallocateProcess(processInCore);
    }else{
      PagingAllocator::getInstance()->deallocateProcess(processInCore);
    }

    ReadyAndFinished::getInstance()->pushProcessToFinished(processInCore);
    removeProcess();

  }else{

    std::chrono::duration<float, std::milli> delayDuration(executionDelay * 1000); 
    std::this_thread::sleep_for(delayDuration);
    idleTicks++;
    getProcess();
  }
   
};

//CPU Core for a Round Robin Scheduler
void CPUCore::RRBehavior(){

  if(processInCore != nullptr){

    for(size_t i = 0; i < quantumCycleCount; i++){

      if(isPrinting){
        std::chrono::duration<float, std::milli> printDelay(5000); 
        std::this_thread::sleep_for(printDelay);
      }

      if(!(processInCore->isFinished)){

        processInCore->executeInstruction();
        activeTicks++;
        std::chrono::duration<float, std::milli> delayDuration(executionDelay * 1000); 
        std::this_thread::sleep_for(delayDuration);

      }else{

        if(!(pageCount > 1)){
          FlatMemoryAllocator::getInstance()->deallocateProcess(processInCore);
        }else{
          PagingAllocator::getInstance()->deallocateProcess(processInCore);
        }

        ReadyAndFinished::getInstance()->pushProcessToFinished(processInCore);
        removeProcess();
        return;

      }

    }

    if(isPrinting){
      std::chrono::duration<float, std::milli> printDelay(5000); 
      std::this_thread::sleep_for(printDelay);
    }

    returnProcess();
    removeProcess();

  }else{

    std::chrono::duration<float, std::milli> delayDuration(executionDelay * 1000); 
    std::this_thread::sleep_for(delayDuration);
    idleTicks++;
    getProcess();

  }

};


//Method to get a process from the ready queue
void CPUCore::getProcess(){

  std::shared_ptr<Process> placeholder = ReadyAndFinished::getInstance()->returnProcess();

  //Checks if the returned value is null
  if(placeholder != nullptr){

    if(!(pageCount > 1)){

      if(FlatMemoryAllocator::getInstance()->allocateProcess(placeholder)){ 
          processInCore = placeholder;
          placeholder.reset();
          isFree = 0;

      }else{

        placeholder->processState = Process::READY;
        ReadyAndFinished::getInstance()->pushProcessToReady(placeholder);
        placeholder.reset();
        isFree = 1;

      }

    }else{

      if(PagingAllocator::getInstance()->allocateProcess(placeholder)){ 
          processInCore = placeholder;
          placeholder.reset();
          isFree = 0;

      }else{

        placeholder->processState = Process::READY;
        ReadyAndFinished::getInstance()->pushProcessToReady(placeholder);
        placeholder.reset();
        isFree = 1;

      }
    }
  }
};

//Removes a process from the core
void CPUCore::removeProcess(){

  processInCore.reset(); 
  isFree = 1;

};

//Returns a process from the core to the ready queue
void CPUCore::returnProcess(){

  processInCore->processState = Process::READY;
  ReadyAndFinished::getInstance()->pushProcessToReady(processInCore);

};


