#include "FlatMemoryAllocator.h"
#include "SchedulerManager.h"
#include <iostream>
#include <unordered_set>

int FlatMemoryAllocator::numPagesIn = 0;
int FlatMemoryAllocator::numPagesOut = 0;

FlatMemoryAllocator *FlatMemoryAllocator::singletonInstance = nullptr;
FlatMemoryAllocator *FlatMemoryAllocator::getInstance() { return singletonInstance; };

FlatMemoryAllocator::FlatMemoryAllocator(int tMemory) : 
  memory(tMemory, std::make_pair(nullptr, false)){

  this->totalMemory = tMemory;
  this->usedMemory = 0;
  this->activeMemory = 0;
  this->inactiveMemory = 0;
  this->backingStore = "flatbacking_store.txt";

};

void FlatMemoryAllocator::initialize(int tMemory){
  singletonInstance = new FlatMemoryAllocator(tMemory);
};

int FlatMemoryAllocator::allocateProcess(std::shared_ptr<Process> process){

  std::lock_guard<std::mutex> lock(mtx);


  //For process memory that is higher than the total memory
  if((process->processMemory) > totalMemory){
    //std::cout << "ifProcessMemoryBigger test" << std::endl;
    return 0;
  }

  //std::cout << "after isBigger than totalMem test" << std::endl;

  if(isInMemory(process)){
    //std::cout << "isInMemory test" << std::endl;
    return 1;
  }

  //std::cout << "after isInMemory test" << std::endl;

  if(allocate(process)){
//    std::cout << "Allocating successful" << std::endl;
    return 1;
  }

  //std::cout << "after allocate test" << std::endl;


    //Checks non-active processes and places them in the backing store
  while(backingStoreOperation()){

    //std::cout << "BackingStore test" << std::endl;
    std::shared_ptr<Process> processToRemove = nullptr;

    // Find the first process in the READY state
    for (const auto& block : memory) {
        const auto& process = block.first;
        bool occupied = block.second;

        if (occupied && (process->processState == Process::READY)) {
            processToRemove = process;
            break;
        }
    }

    // If a READY process is found, remove all its blocks

    if(processToRemove != nullptr){

      process->serializeToFile(backingStore);

      for (auto& block : memory) {
          if (block.first == processToRemove) {
              block.first = nullptr; // Clear the process pointer
              block.second = false;   // Mark the block as free
          }
      }
      numPagesOut++;
    }else{
      return 0;
    }


    if(allocate(process)){
      return 1;
    }
  }

  return 0;
};


/*int FlatMemoryAllocator::allocateProcessFirst(std::shared_ptr<Process> process){

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
*/



void FlatMemoryAllocator::deallocateProcess(std::shared_ptr<Process> process){

  std::lock_guard<std::mutex> lock(mtx);
  
  for (auto& block : memory) {
      if (block.first == process) {
          block.first = nullptr; // Clear the process pointer
          block.second = false;   // Mark the block as free
      }
  }
  numPagesOut++;

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
  int allocatedMemory = getActiveMemory();
  double memoryPercentage = ((double)allocatedMemory / (double)totalMemory) * 100;


  std::cout << "\nCPU-Util: " << CPUPercentage << std::endl;
  std::cout << "Memory Usage: " << allocatedMemory << "KB / " << totalMemory << "KB" << std::endl;
  std::cout << "Memory Utilization: " << memoryPercentage << "%\n" << std::endl;
  std::cout << "Running processes and memory usage: \n" << std::endl;

  std::unordered_set<std::shared_ptr<Process> > processingProcesses;

    for (const auto& block : memory) {
        const auto& process = block.first;

        if (process && process->processState == Process::PROCESSING) {
            processingProcesses.insert(process);
        }
    }

  for(auto i : processingProcesses){

    std::cout << i->processName << " " << i->processMemory << std::endl; 

  }

  std::cout << "\n";
};


void FlatMemoryAllocator::vmStat(){

  std::lock_guard<std::mutex> lock(mtx);

  int externalFragmentation = getExternalFragmentation();
  int activeMemory = getActiveMemory();
  int usedMemory =  activeMemory + externalFragmentation;

  std::cout << "\n" << totalMemory << "KB total memory" << std::endl;
  std::cout << usedMemory << "KB used memory" << std::endl;
  std::cout << activeMemory << "KB active memory" << std::endl;
  std::cout << externalFragmentation << "KB inactive memory" << std::endl;
  std::cout << CPUCore::idleTicks << " idle cpu ticks" << std::endl;
  std::cout << CPUCore::activeTicks << " active cpu ticks" << std::endl;
  std::cout << (CPUCore::activeTicks + CPUCore::idleTicks) << " total cpu ticks" << std::endl;
  std::cout << numPagesIn << " num paged in" << std::endl;
  std::cout << numPagesOut << " num paged out" << "\n" << std::endl;

};



int FlatMemoryAllocator::backingStoreOperation() {

    std::unordered_set<std::shared_ptr<Process> > readyProcesses;
    for (const auto& block : memory) {
        const auto& process = block.first;
        bool occupied = block.second;

        if (occupied && process->processState == Process::READY) {
            readyProcesses.insert(process);
        }
    }
    
    return readyProcesses.size();
}

int FlatMemoryAllocator::allocate(std::shared_ptr<Process> process){


  //std::cout << "In allocate function " << std::endl;
  int freeCount = 0;
  int startIndex = 0;

  // Find a contiguous block of free memory
  for (int i = 0; i < memory.size(); ++i) {
      if (!memory[i].second) { // If the memory unit is free
          if (freeCount == 0) startIndex = i;
          freeCount++;
          if (freeCount == process->processMemory) break;
      } else {
          freeCount = 0;
      }
  }

  //If it encounters a free contiguous block
  if (freeCount >= process->processMemory){
    for (int i = startIndex; i < startIndex + process->processMemory; ++i) {
      memory[i] = std::make_pair(process, true);
    }
    numPagesIn++;
    process->processState = Process::PROCESSING;
    return 1;
  }else{
    return 0;
  }

};

int FlatMemoryAllocator::getExternalFragmentation(){

  int firstAllocatedIndex = memory.size();
  int lastAllocatedIndex = 0;

  // Identify the first and last allocated blocks
  for (int i = 0; i < memory.size(); ++i) {
      if (memory[i].first != nullptr) {
          if (i < firstAllocatedIndex) firstAllocatedIndex = i;
          if (i > lastAllocatedIndex) lastAllocatedIndex = i;
      }
  }


  std::cout << "in external fragmentation" << std::endl;
  // If no blocks are allocated, return 0
  if (firstAllocatedIndex == memory.size()) {
      return 0;
  }

  // Count free blocks between the first and last allocated blocks
  int freeBlocks = 0;
  for (int i = firstAllocatedIndex; i <= lastAllocatedIndex; ++i) {
      if (memory[i].second == false) {
          freeBlocks++;
      }
  }

  return freeBlocks;
};


int FlatMemoryAllocator::getActiveMemory(){

  int activeMemory = 0;
  for (int i = 0; i < memory.size(); ++i) {
      if (memory[i].first != nullptr) {
        activeMemory++;
      }
  }
  std::cout << "in active memory" << std::endl;

  return activeMemory;
};


int FlatMemoryAllocator::isInMemory(std::shared_ptr<Process> process){

  for (int i = 0; i < memory.size(); ++i) {
      if (memory[i].first == process) {
        process->processState = Process::PROCESSING;
        return 1;
      }
  }
  return 0;
};





