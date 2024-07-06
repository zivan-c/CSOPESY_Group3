#include "Process.h"
#include "Console.h"
#include <string>
#include <iostream>


class ProcessScreen : public Console {


public:

	static ProcessScreen* getInstance();
	void attachProcessToScreen(std::shared_ptr<Process> process);
	void processUserInput(std::string input);
	static void initialize();
	void runScreen();

private:
	bool isRunning;
	std::shared_ptr<Process> screenProcess;
	static ProcessScreen* singletonInstance;
};


//should do ProcessScreen::initialize alongside the cpuscheduler

//in process-s<process_name>
//CPUScheduler::getInstance()->createProcess(process_name);
//std::shared_ptr<Process> process =  CPUScheduler::getInstance()->getProcessPointer(process_name);
//if (process != nullptr)
//ProcessScreen::getInstance()->attachProcessToScreen(process);
//ProcessScreen::getInstnace()->runScreen();
//else
//std::cout << "Process: " << process_name << " not found." << std::endl;


//in process-r<process_name>
//std::shared_ptr<Process> process =  CPUScheduler::getInstance()->getProcessPointer(process_name);
//if (process != nullptr)
//ProcessScreen::getInstance()->attachProcessToScreen(process);
//ProcessScreen::getInstance()->runScreen();
//else
//std::cout << "Process: " << process_name << " not found." << std::endl;
//