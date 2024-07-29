#include "FlatMemoryAllocator.h"
#include "SchedulerManager.h"
#include <iostream>
#include <algorithm>


FlatMemoryAllocator *FlatMemoryAllocator::singletonInstance = nullptr;
FlatMemoryAllocator *FlatMemoryAllocator::getInstance() { return singletonInstance; };

void FlatMemoryAllocator::initialize(int tMemory){

  singletonInstance = new FlatMemoryAllocator();
  singletonInstance->totalMemory = tMemory;
  singletonInstance->allocatedMemory = 0;
  singletonInstance->backingStore = "backing_store.txt";

};


int FlatMemoryAllocator::allocateProcess(std::shared_ptr<Process> process){

  std::lock_guard<std::mutex> lock(mtx);
  int freeMemory = (totalMemory - allocatedMemory);
  
  if(freeMemory >= process->processMemory){

    allocatedMemory += process->processMemory;
    processesInMemory.push_back(process);
    return 1;

  }else{

    if(!processesInMemory.empty()){

      std::shared_ptr<Process> placeholder = nullptr;

      for(auto i : processesInMemory){
        if(i->processState == Process::READY){
          placeholder = i;
        }
      }


      if(placeholder != nullptr){ //IF A PROCESS IN READY STATE IS FOUND

        int availableSpace = totalMemory - (allocatedMemory - placeholder->processMemory);
        
        if(availableSpace >= process->processMemory){

          placeholder->serializeToFile(backingStore);

          for(auto it = processesInMemory.begin(); it != processesInMemory.end(); ) {
            if ((*it)->processID == placeholder->processID) {
              allocatedMemory -= (*it)->processMemory;
              it = processesInMemory.erase(it);
            } else {
              ++it;
            }
          }

          allocatedMemory += process->processMemory;
          processesInMemory.push_back(process);
          return 1;

        }else{ return 0;}
      }else {return 0;}
    }else {return 0;}
  }   
  return 0;
};




void FlatMemoryAllocator::deallocateProcess(int pID){

  std::lock_guard<std::mutex> lock(mtx);

  for(auto it = processesInMemory.begin(); it != processesInMemory.end(); ) {
    if ((*it)->processID == pID) {
      allocatedMemory -= (*it)->processMemory;
      it = processesInMemory.erase(it);
    } else {
      ++it;
    }
  }

};

void FlatMemoryAllocator::printProcessesInMemory(){

  std::lock_guard<std::mutex> lock(mtx);

  int coresUsed = 0;
  for (auto& i : SchedulerManager::getInstance()->coreList) {
    if (!(i->isFree)) {
      coresUsed++;
    }
  }
  double CPUPercentage = ((double)coresUsed / (double)SchedulerManager::getInstance()->CPUCoreCount) * 100;
  double memoryPercentage = ((double)allocatedMemory / (double)totalMemory) * 100;


  std::cout << "\nCPU-Util: " << CPUPercentage << std::endl;
  std::cout << "Memory Usage: " << allocatedMemory << "KB / " << totalMemory << "KB" << std::endl;
  std::cout << "Memory Utilization: " << memoryPercentage << "%\n" << std::endl;
  std::cout << "Running processes and memory usage: \n" << std::endl;

  for(auto i : processesInMemory){

  std::cout << i->processName << " " << i->processMemory << std::endl; 


  }

  std::cout << "\n";
};


void FlatMemoryAllocator::vmStat(){

  std::lock_guard<std::mutex> lock(mtx);

  int activeMemory = 0;
  int inactiveMemory = 0;

  for(auto i : processesInMemory){

    if(i->processState == Process::PROCESSING){

      activeMemory += i->processMemory;

    }else{

      inactiveMemory += i->processMemory;

    }
  }

  std::cout << "\n" << totalMemory << "KB total memory" << std::endl;
  std::cout << allocatedMemory << "KB used memory" << std::endl;
  std::cout << activeMemory << "KB active memory" << std::endl;
  std::cout << inactiveMemory << "KB inactive memory" << std::endl;
  std::cout << CPUCore::idleTicks << " idle cpu ticks" << std::endl;
  std::cout << CPUCore::activeTicks << " active cpu ticks" << std::endl;
  std::cout << (CPUCore::activeTicks + CPUCore::idleTicks) << " total cpu ticks" << std::endl;
  std::cout << " num paged in" << std::endl;
  std::cout << " num paged out" << "\n" << std::endl;

};

