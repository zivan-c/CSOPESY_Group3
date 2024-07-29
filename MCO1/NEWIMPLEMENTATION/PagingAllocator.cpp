#include "PagingAllocator.h"

PagingAllocator *PagingAllocator::singletonInstance = nullptr;
PagingAllocator *PagingAllocator::getInstance() { return singletonInstance; };

void PagingAllocator::initialize(int tMemory, int pSize){

  singletonInstance = new PagingAllocator();
  singletonInstance->totalMemory = tMemory;
  singletonInstance->allocatedMemory = 0;
  singletonInstance->backingStore = "pagingbacking_store.txt";
  singletonInstance->initializePages(tMemory, pSize);

};

void PagingAllocator::initializePages(int tMemory, int pSize){
  pageCount = tMemory / pSize;
};

