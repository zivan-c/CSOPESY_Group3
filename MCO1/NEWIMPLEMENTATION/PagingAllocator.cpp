#include "PagingAllocator.h"
#include "SchedulerManager.h"
#include <unordered_set>
#include <iostream>
#include <string>

int PagingAllocator::numPagesIn = 0;
int PagingAllocator::numPagesOut = 0;

PagingAllocator *PagingAllocator::singletonInstance = nullptr;
PagingAllocator *PagingAllocator::getInstance() { return singletonInstance; };

PagingAllocator::PagingAllocator(int tMemory, int fSize, int pCount){

  int size = tMemory / fSize;
  memory = std::vector<std::pair<std::shared_ptr<Process>, bool>>(size, {nullptr, false});
  
  frameSize = fSize;
  pageCount = pCount;
  totalMemory = tMemory;
  frameCount = size;
  usedMemory = 0; //total used memory including external fragmentation
  activeMemory = 0; //total memory of active processes
  inactiveMemory = 0; //all external fragmentation
  backingStore = "pagingbacking_store.txt";

};

void PagingAllocator::initialize(int tMemory, int fSize, int pCount){
  singletonInstance = new PagingAllocator(tMemory, fSize, pCount);
};




void PagingAllocator::deallocateProcess(std::shared_ptr<Process> process){

  std::lock_guard<std::mutex> lock(mtx);
  
  for (auto& block : memory) {
      if (block.first == process) {
          block.first = nullptr; // Clear the process pointer
          block.second = false;   // Mark the block as free
      }
  }
  numPagesOut += process->pageCount;

};

int PagingAllocator::allocateProcess(std::shared_ptr<Process> process){

  std::lock_guard<std::mutex> lock(mtx);


  //std::cout << "in pagingallocateprocess" << std::endl;

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
      numPagesOut += pageCount;
    }else{
      return 0;
    }


    if(allocate(process)){
      return 1;
    }
  }

  return 0;
};


int PagingAllocator::allocate(std::shared_ptr<Process> process){

  //std::cout << "In allocate function " << std::endl;
  int freeCount = 0;
  int startIndex = 0;

  // Find a contiguous block of free memory
  for (int i = 0; i < memory.size(); ++i) {
      if (!memory[i].second) { // If the memory unit is free
          if (freeCount == 0) startIndex = i;
          freeCount++;
          if (freeCount == process->pageCount) break;
      } else {
          freeCount = 0;
      }
  }

  //If it encounters a free contiguous block
  if (freeCount >= process->pageCount){
    for (int i = startIndex; i < startIndex + process->pageCount; ++i) {
      memory[i] = std::make_pair(process, true);
    }
    numPagesIn += pageCount;
    process->processState = Process::PROCESSING;
    //std::cout << "successful allocation" << std::endl;
    return 1;
  }else{
    //std::cout << "unsuccessful allocation" << std::endl;
    return 0;
  }

};

int PagingAllocator::backingStoreOperation() {

  std::unordered_set<std::shared_ptr<Process> > readyProcesses;
  for (const auto& block : memory) {
      const auto& process = block.first;
      bool occupied = block.second;

      if (occupied && process->processState == Process::READY) {
          readyProcesses.insert(process);
      }
  }
  
  return readyProcesses.size();

};


int PagingAllocator::isInMemory(std::shared_ptr<Process> process){

  for (int i = 0; i < memory.size(); ++i) {
      if (memory[i].first == process) {
        process->processState = Process::PROCESSING;
        return 1;
      }
  }
  return 0;
};


int PagingAllocator::getExternalFragmentation(){

  int firstAllocatedIndex = memory.size();
  int lastAllocatedIndex = 0;

  // Identify the first and last allocated blocks
  for (int i = 0; i < memory.size(); ++i) {
      if (memory[i].first != nullptr) {
          if (i < firstAllocatedIndex) firstAllocatedIndex = i;
          if (i > lastAllocatedIndex) lastAllocatedIndex = i;
      }
  }
  //std::cout << "in external fragmentation" << std::endl;
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

  int freeSize = freeBlocks * frameSize;

  return freeSize;
};


int PagingAllocator::getActiveMemory(){

  std::unordered_set<std::shared_ptr<Process> > readyProcesses;
  for (const auto& block : memory) {
      const auto& process = block.first;
      bool occupied = block.second;

      if (occupied) {
          readyProcesses.insert(process);
      }
  }

  int activeMemory = 0;
  
  for (const auto& process : readyProcesses) {
    if (process) {
      activeMemory += process->processMemory;
    }
  }

  return activeMemory;
  
};



void PagingAllocator::printProcessesInMemory(){

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


void PagingAllocator::vmStat(){

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








