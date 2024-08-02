#include "CPUCore.h"
#include "ReadyAndFinished.h"
#include "FlatMemoryAllocator.h"
#include "PagingAllocator.h"

size_t CPUCore::activeTicks = 0;
size_t CPUCore::idleTicks = 0;
int CPUCore::isPrinting = 0;

CPUCore::CPUCore(int coreID, float executionDelay, int quantumCycleCount, int pCount){

  this->coreID = coreID;
  this->executionDelay = executionDelay;
  this->quantumCycleCount = quantumCycleCount;
  this->pageCount = pCount;
  isRunning = 0;

};

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


void CPUCore::FCFSBehavior(){

  if(processInCore != nullptr){

    while(!(processInCore->isFinished)){

      processInCore->executeInstruction();
      activeTicks++;
      std::chrono::duration<float, std::milli> delayDuration(executionDelay * 1000); 
      std::this_thread::sleep_for(delayDuration);

    }
    //(TODO) Add to finished processes, done by r
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

    //(TODO) set the process state to READY, done by returnProcess();
    returnProcess();
    removeProcess();

  }else{

    std::chrono::duration<float, std::milli> delayDuration(executionDelay * 1000); 
    std::this_thread::sleep_for(delayDuration);
    idleTicks++;
    getProcess();

  }

};

void CPUCore::getProcess(){

  std::shared_ptr<Process> placeholder = ReadyAndFinished::getInstance()->returnProcess();

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

void CPUCore::removeProcess(){

  processInCore.reset(); 
  isFree = 1;

};

void CPUCore::returnProcess(){

  processInCore->processState = Process::READY;
  ReadyAndFinished::getInstance()->pushProcessToReady(processInCore);

};


