#pragma once

#include "Process.h"
#include <mutex>
#include <vector>

class PagingAllocator {

public:

	int totalMemory;
	int pageCount;
	int frameCount;
	int frameSize;
	int usedMemory; //total used memory including external fragmentation
	int activeMemory; //total memory of active processes
	int inactiveMemory; //all external fragmentation
	static int numPagesIn;
	static int numPagesOut;

	std::string backingStore;

	std::vector<std::shared_ptr<Process> > processesInMemory;


	void static initialize(int tMemory, int fSize, int pCount);
	static PagingAllocator* getInstance();

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
	static PagingAllocator* singletonInstance;
	std::vector<std::pair<std::shared_ptr<Process>, bool> > memory;
	PagingAllocator(int tMemory, int fSize, int pCount);


};