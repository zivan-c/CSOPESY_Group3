#include "CPUCore.h"
#include "CPUScheduler.h"
#include "Process.h"
#include <chrono>
#include <semaphore>
#include <thread> 
#include <ctime>
#include "FirstFit.h"
#include "ReadyQueue.h"


std::counting_semaphore<1> CPUCore::sem(2);

//Constructor for CPUCore with the arguments and sets it to available
CPUCore::CPUCore(int id, float executionDelay, int quantumCycles){

  this->cpuCoreID = id;
  this->executionDelay = executionDelay;
  this->quantumCycles = quantumCycles;
  this->isAvailable = true;

};

//Executes an instruction of the process inside
void CPUCore::executeProcess(){

  if(processInCPUCore){

    this->processInCPUCore->executeInstruction();

  }

};

//Returns the pointer for the process inside of it
std::shared_ptr<Process> CPUCore::getProcessinCPUCore(){

  if(processInCPUCore){

    return processInCPUCore;

  }else{

    return nullptr;

  }

};

void CPUCore::runCore(){

  coreThread = std::thread([this](){
  this->isRunning = true;

    //Different behaviors if RR is the scheduler
    while(isRunning){
        if(quantumCycles){
          RRCPUBehavior();
        }
        else{
          normalCPUBehavior();
        }
    };
  });

  coreThread.detach();

};

      
//Normal CPU Behavior, it is not in charge of getting new processes from the ready queue,
//the scheduler handles it
void CPUCore::normalCPUBehavior(){

  if(processInCPUCore){
    if(processInCPUCore->getProcessState() == Process::ProcessState::PROCESSING){

      executeProcess();
      std::chrono::duration<float, std::milli> delayDuration(executionDelay * 1000); 
      std::this_thread::sleep_for(delayDuration);

    }else{
      addToFinishedList();
    }   
  }
};

//RR scheduler CPU Behavior, where the core itself switches the processes from itself and the 
//ready queue, done so, since the quantum cycles of each core is not synchronized.
//
//
//
void CPUCore::RRCPUBehavior() {


  //let all getting from the readyqueue be in a semaphore, route it so it becomes so.
  //
    if (processInCPUCore != nullptr) {
        for (int i = 0; i < quantumCycles; i++) {
            if (processInCPUCore->getProcessState() == Process::ProcessState::PROCESSING) {
                executeProcess();
                std::chrono::duration<float, std::milli> delayDuration(executionDelay * 1000);
                std::this_thread::sleep_for(delayDuration);
            } else {


                if(processInCPUCore->getProcessState() == Process::ProcessState::FINISHED){

                    if (FirstFit::getInstance()->isPrinting){
                      std::chrono::duration<float, std::milli>
                   delayDuration(1500);
                    }

                    //FirstFit::getInstance()->deallocate(processInCPUCore, processInCPUCore->memoryAmount);
                    addToFinishedList();
                    


                }
        
              }
            }

        CPUCore::sem.acquire();
        if (FirstFit::getInstance()->isPrinting){
          std::chrono::duration<float, std::milli>
       delayDuration(1500);
        }
        
        returnProcesstoReadyQueue();
        CPUCore::sem.release();

    } else {

        CPUCore::sem.acquire();

        if (ReadyQueue::getInstance()->isReadyQueueAvailable()) {

            getProcessFromReadyQueue();
        }
        
        CPUCore::sem.release();
    }


};


//Gets a process from the readyqueue and assigns it to the core
void CPUCore::getProcessFromReadyQueue(){
  //

  if(ReadyQueue::getInstance()->isReadyQueueAvailable()){

    std::shared_ptr<Process> placeholder = ReadyQueue::getInstance()->removeProcessFromReadyQueue(); 
    /*if(placeholder != nullptr){

      if(!FirstFit::getInstance()->allocate(placeholder, placeholder->memoryAmount)){
        
        ReadyQueue::getInstance()->pushToReadyQueue(placeholder);
        this->processInCPUCore = nullptr;

      }else{

        this->processInCPUCore = placeholder; 
        this->isAvailable = false;

        if(processInCPUCore){
          processInCPUCore->setProcessState(Process::ProcessState::PROCESSING);
        }

      }
    }*/

    if (placeholder != nullptr) {
    this->processInCPUCore = placeholder;
    this->isAvailable = false;
    processInCPUCore->setProcessState(Process::ProcessState::PROCESSING);
    }

  } else {

    this->processInCPUCore = nullptr;

  }
};


//Adds the process to the finished list.
void CPUCore::addToFinishedList(){

    if(processInCPUCore){

      CPUScheduler::getInstance()->addProcessToFinishedProcesses(processInCPUCore);
      removeProcessinCPUCore();

    }

};

void CPUCore::returnProcesstoReadyQueue(){

  //std::lock_guard<std::mutex> lock(queueMutex);
  //
    if(processInCPUCore){

      //FirstFit::getInstance()->deallocate(processInCPUCore, processInCPUCore->memoryAmount);
      processInCPUCore->setProcessState(Process::ProcessState::READY);
      ReadyQueue::getInstance()->pushToReadyQueue(processInCPUCore); 
      removeProcessinCPUCore();

    }

};


//Resets the process in the CPU core.
void CPUCore::removeProcessinCPUCore(){

  if(processInCPUCore){
    processInCPUCore.reset(); //Now, the CPUCore is empty;
    this->isAvailable = true; //CPU Core is available to be used
  //for checking
  }
};

int CPUCore::getCoreID(){

  return this->cpuCoreID;

};

int CPUCore::isCoreFree(){

  //std::lock_guard<std::mutex> lock(queueMutex); // Lock the mutex
  if (this->isAvailable){
    return 1;
  }else{
    return 0;
  }
};

