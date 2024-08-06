#pragma once

#include "Process.h"

#include <memory>
#include <string>

class ProcessScreen {

public:

	void run(std::shared_ptr<Process> process);
	void static initialize();
	static ProcessScreen* getInstance();
	int isRunning;

private:

	static ProcessScreen* singletonInstance;
	void inputProcess(std::string input, std::shared_ptr<Process> process);

};