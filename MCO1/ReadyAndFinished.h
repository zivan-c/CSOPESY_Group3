#pragma once

#include <vector>
#include <mutex>

#include "Process.h"

class ReadyAndFinished {

public:

	void pushProcessToReady(std::shared_ptr<Process> process);
	void pushProcessToFinished(std::shared_ptr<Process> process);
	std::shared_ptr<Process> returnProcess();
	std::vector<std::shared_ptr<Process> > finishedList;
	static void initialize();
	static ReadyAndFinished* getInstance();


private:

	std::mutex mtx;
	static ReadyAndFinished* singletonInstance;
	std::vector<std::shared_ptr<Process> > readyQueue;

};