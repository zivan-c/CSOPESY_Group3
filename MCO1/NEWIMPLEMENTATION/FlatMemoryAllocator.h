#pragma once

#include <vector>
#include <memory>
#include <mutex>

#include "Process.h"

class FlatMemoryAllocator{

public:

  int totalMemory;
  
  //NEW IMPLEMENTATION
  int usedMemory; //total used memory including external fragmentation
  int activeMemory; //total memory of active processes
  int inactiveMemory; //all external fragmentation
  static int numPagesIn;
  static int numPagesOut;
  
  std::string backingStore;

  std::vector<std::shared_ptr<Process> > processesInMemory;

  
  void static initialize(int tMemory);
  static FlatMemoryAllocator* getInstance();

  int allocateProcess(std::shared_ptr<Process>);
  int allocate(std::shared_ptr<Process>);
  int isInMemory(std::shared_ptr<Process> process);
  void deallocateProcess(std::shared_ptr<Process> process);
  void printProcessesInMemory();
  int getExternalFragmentation();
  int getActiveMemory();
  void vmStat();
  int backingStoreOperation();

private:
  std::mutex mtx;
  static FlatMemoryAllocator* singletonInstance;
  std::vector<std::pair<std::shared_ptr<Process>, bool> > memory;
  FlatMemoryAllocator(int tMemory);



};
