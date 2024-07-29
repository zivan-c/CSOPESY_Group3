#include "FirstFit.h"
#include <iostream>
#include <vector>
#include "Process.h"
#include <chrono>
#include <sstream>
#include <string>
#include "CPUScheduler.h"

FirstFit *FirstFit::singletonInstance = nullptr;
FirstFit *FirstFit::getInstance() { return singletonInstance; };

FirstFit::FirstFit(int size) 
    : memory(size, std::make_pair(nullptr, false)) {
    this->memorySize = size;
    this->allocatedMemory = 0;
    this->processesinMemory = 0;
    this->isPrinting = false;
}

void FirstFit::initialize(int memoryAmount){

  singletonInstance = new FirstFit(memoryAmount);
  singletonInstance->allow = 1;

};

bool FirstFit::allocate(std::shared_ptr<Process> process, int processMemoryAmount) {
    // Returns if allocation is successful or not 
  while(true){
    if (!isPrinting){
      int freeCount = 0;
      int startIndex = 0;

      // Find a contiguous block of free memory
      for (size_t i = 0; i < memory.size(); ++i) {
          if (!memory[i].second) { // If the memory unit is free
              if (freeCount == 0) startIndex = i;
              freeCount++;
              if (freeCount == processMemoryAmount) break;
          } else {
              freeCount = 0;
          }
      }

      if (freeCount < processMemoryAmount) return false; // Not enough contiguous memory

      // Allocate the memory
      for (size_t i = startIndex; i < startIndex + processMemoryAmount; ++i) {
          memory[i] = std::make_pair(process, true);
      }

      allocatedMemory += processMemoryAmount;
      processesinMemory++;
      return true;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
};

bool FirstFit::deallocate(std::shared_ptr<Process> process, int processMemoryAmount) {
    
    while(true){
      if (!isPrinting){

        int deallocatedCount = 0;
      
        for (auto& unit : memory) {
          if (unit.first == process) {
              unit = std::make_pair(nullptr, false);
              deallocatedCount++;
              if (deallocatedCount == processMemoryAmount) break;
          }
        }

        processesinMemory--;
        allocatedMemory -= processMemoryAmount;
        return true;

      }

      std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}


int FirstFit::totalExternalFragmentation(){

  int externalFragmentationSize = 0;

 for(size_t i = 0; i < memory.size(); i++){
    if(!memory[i].second){
      externalFragmentationSize++;
    }
  }

  return externalFragmentationSize;

};


std::string FirstFit::getDateAndTime(){

  auto now = std::chrono::system_clock::now();
  std::time_t calendarTime = std::chrono::system_clock::to_time_t(now);

  std::tm local_tm;
  localtime_r(&calendarTime, &local_tm); 

  std::ostringstream oss;
  oss << std::put_time(&local_tm, "(%m/%d/%Y %H:%M:%S)"); 

  return oss.str();

};

void FirstFit::printMemoryProgress(){

  std::cout << "\n==========================================\n" << std::endl;
  std::cout << "Running processes and memory usage:\n" << std::endl;
  std::cout << "------------------------------------------\n" << std::endl;
    
  int allMemory = 0;
  for (size_t i = 0; i < memory.size(); ++i) {
    if (memory[i].second) { // If memory block is allocated
      std::cout << memory[i].first->getProcessName() << " " << memory[i].first->memoryAmount << "KB" << std::endl;
      allMemory += memory[i].first->memoryAmount;
      // Find the end index of the allocated memory block
      size_t endIndex = i;
      while (endIndex < memory.size() && memory[endIndex].second) {
        ++endIndex;
      }
      i = endIndex; // Skip to the end of this allocated block
    }
  }

  std::cout << "\nMemory Usage: " << allMemory << " / " << memorySize << std::endl;
  double memoryPercentage = ((double)allMemory / (double)memorySize) * 100;
  std::cout << "Memory Util: " << memoryPercentage << "%" << std::endl;
  std::cout << "\n------------------------------------------\n" << std::endl;

};




