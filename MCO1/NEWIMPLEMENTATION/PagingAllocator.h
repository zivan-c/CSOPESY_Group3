#pragma once

#include "Process.h"

#include <vector>

class PagingAllocator {

public:

  int totalMemory;
  int allocatedMemory;
  int pageSize;
  int pageCount;
  std::string backingStore;

  void static initialize(int tMemory, int pSize);
  static PagingAllocator* getInstance();
  void initializePages(int tMemory, int pSize);

private:

  std::mutex mtx;
  static PagingAllocator* singletonInstance;

};
