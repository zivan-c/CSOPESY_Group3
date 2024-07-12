#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Process.h"
#include "CPUCore.h"


//Scheduler class that each algorithm inherits for easier linkage
class Scheduler {

public:
	virtual void runScheduler() = 0;
	int isRunning = 1;
private:

};