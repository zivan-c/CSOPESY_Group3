#pragma once

#include <vector>
#include <memory>
#include <mutex>

#include "Process.h"

class FlatMemoryAllocator{

public:

  int totalMemory;
  int allocatedMemory;
  std::string backingStore;

  std::vector<std::shared_ptr<Process> > processesInMemory;

  void static initialize(int tMemory);
  static FlatMemoryAllocator* getInstance();

  int allocateProcess(std::shared_ptr<Process>);

  void deallocateProcess(int pID);
  void printProcessesInMemory();
  void vmStat();

private:
  std::mutex mtx;
  static FlatMemoryAllocator* singletonInstance;


};
