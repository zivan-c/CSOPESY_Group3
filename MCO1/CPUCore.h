#pragma once
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <thread>
#include "Scheduler.h"
#include <mutex>

class CPUCore {

public:

	CPUCore(int id, float executionDelay);
	CPUCore(int id, float executionDelay, int quantumCycles);
	~CPUCore() = default;

	bool isRunning = false;

	int isCoreFree();
	void runCore();
	std::string getDateandTime();
	void executeProcess();
	std::shared_ptr<Process> getProcessinCPUCore();
	int getCoreID();

	void getProcessFromReadyQueue();
	void attachProcesstoCPUCore();
	void returnProcesstoReadyQueue();
	void addToFinishedList();
	void removeProcessinCPUCore();

private:

	std::thread coreThread;
	std::mutex queueMutex;
	bool isAvailable = true;

	int cpuCoreID;
	int quantumCycles;
	float executionDelay;
	std::shared_ptr<Process> processInCPUCore;

	void normalCPUBehavior();
	void RRCPUBehavior();

};