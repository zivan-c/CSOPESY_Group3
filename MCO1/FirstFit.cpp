#include "FirstFit.h"
#include <iostream>
#include <vector>
#include "Process.h"
#include <chrono>
#include <mutex>
#include <fstream>
#include <sstream>
#include <string>
#include "CPUScheduler.h"

FirstFit *FirstFit::singletonInstance = nullptr;
FirstFit *FirstFit::getInstance() { return singletonInstance; };

FirstFit::FirstFit(int size) 
    : memory(size, std::make_pair("", false)) {
    this->memorySize = size;
    this->allocatedMemory = 0;
    this->processesinMemory = 0;
}

void FirstFit::initialize(int memoryAmount){

  singletonInstance = new FirstFit(memoryAmount);

};

bool FirstFit::allocate(std::string processName, int processMemoryAmount) {
    // Returns if allocation is successful or not 
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
        memory[i] = std::make_pair(processName, true);
    }

    //std::cout << processName << " has been allocated with: " << processMemoryAmount << std::endl;
    allocatedMemory += processMemoryAmount;
    processesinMemory++;
    return true;
};

void FirstFit::deallocate(std::string processName, int processMemoryAmount) {
    int deallocatedCount = 0;
    
    for (auto& unit : memory) {
        if (unit.first == processName) {
            unit = std::make_pair("", false);
            deallocatedCount++;
            if (deallocatedCount == processMemoryAmount) break;
        }
    }

    processesinMemory--;
    //std::cout << processName << " has been DEALLOCATED with: " << processMemoryAmount << std::endl;
  
    allocatedMemory -= processMemoryAmount;
}


int FirstFit::totalExternalFragmentation(){

  //std::lock_guard<std::mutex> lock(queueMutex); 
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

  //std::lock_guard<std::mutex> lock(queueMutex); // Lock the mutex
  //
  //

  std::ofstream memoryFile; 
  std::stringstream ss;
  ss << "memory_stamp_<" << CPUScheduler::getInstance()->quantumCycleAmount << ">.txt";
  std::string memoryFileName = ss.str(); 
  memoryFile.open(memoryFileName);

  if(!memoryFile){
  
    std::cout << "File could not be opened." << std::endl;

  }else{

    std::string dateAndTime = this->getDateAndTime();

    memoryFile << "Timestamp: " << dateAndTime << std::endl; 
    memoryFile << "Number of processes in memory: " << FirstFit::getInstance()->processesinMemory << std::endl;

    //place here the total amount of unallocated memory remaining
    memoryFile << "Total external fragmentation in KB: " <<
      FirstFit::getInstance()->totalExternalFragmentation() << "\n" << std::endl;

    memoryFile << "----end---- = " << FirstFit::getInstance()->memorySize << "\n" << std::endl;
    for (size_t i = 0; i < memory.size(); ++i) {
      if (memory[i].second) { // If memory block is allocated
          memoryFile << "Process Name: " << memory[i].first << "\n";
          memoryFile << "Start Index: " << i << "\n";
        // Find the end index of the allocated memory block
          size_t endIndex = i;
          while (endIndex < memory.size() && memory[endIndex].second) {
              ++endIndex;
          }
          memoryFile << "End Index: " << endIndex - 1 << "\n\n";
          i = endIndex; // Skip to the end of this allocated block
      }
    }

   /*for (size_t i = 0; i < memory.size(); ++i) {
      if (memory[i].second) { // If memory block is allocated
        memoryFile << "Process Name: " << memory[i].first << "\n";
        memoryFile << "Start Index: " << i << "\n";
                // Find the end index of the allocated memory block
        size_t endIndex = i;
        while (endIndex < memory.size() && memory[endIndex].second) {
              ++endIndex;
        }
        memoryFile << "End Index: " << endIndex - 1 << "\n\n";
            i = endIndex - 1; // Skip to the end of this allocated block
        }
      }  
  */
 
    //per process in the memory
    
    memoryFile << "----start----- = 0" << std::endl;

  }




};




