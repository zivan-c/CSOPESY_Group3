#include "Process.h"
#include "Console.h"
#include <string>
#include <iostream>

//should inherit console class
class ProcessScreen {

public:

	void attachProcessToScreen(std::shared_ptr<Process> process);
	void processUserInput(std::string input);
	void runConsole(); //should have override keyword

private:
	bool isRunning;
	std::shared_ptr<Process> screenProcess;
};