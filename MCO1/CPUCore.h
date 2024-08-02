#pragma once

#include "Process.h"

#include <thread>
#include <mutex>

class CPUCore {


public:

	std::shared_ptr<Process> processInCore;

	int coreID;
	float executionDelay;
	int quantumCycleCount;
	int isFree;
	int isRunning;
	int pageCount;


	//For ticks
	static size_t idleTicks;
	static size_t activeTicks;
	static int isPrinting;
	size_t totalTicks;


	CPUCore(int coreID, float executionDelay, int quantumCycleCount, int pageCount);

	void run();

	void FCFSBehavior();
	void RRBehavior();

	void getProcess();
	void returnProcess();
	void removeProcess();

private:

	std::thread cpuCoreThread;
	std::mutex mtx;

};