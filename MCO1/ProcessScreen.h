#include "Process.h"
#include "Console.h"
#include <string>
#include <iostream>

//should inherit console class
class ProcessScreen:public Console{

public:
	ProcessScreen(); // Add the default constructor
	~ProcessScreen();

	void attachProcessToScreen(std::shared_ptr<Process> process);
	void processUserInput(std::string input);

	void activate() override; //should have override keyword
	void display() override;
	void process() override;

private:
	bool isRunning;
	std::shared_ptr<Process> screenProcess;
};


//in process-s<process_name>
//CPUScheduler::getInstance()->createProcess(process_name);
//std::shared_ptr<Process> process =  CPUScheduler::getInstance()->getProcessPointer(process_name);
//if (process != nullptr)
//access the process pointer then, attachProcessToScreen(process);
//access the process pointer then, runScreen();
//else
//std::cout << "Process: " << process_name << " not found." << std::endl;


//in process-r<process_name>
//std::shared_ptr<Process> process =  CPUScheduler::getInstance()->getProcessPointer(process_name);
//if (process != nullptr)
//access the process pointer then, attachProcessToScreen(process);
//access the process pointer then, runScreen();
//else
//std::cout << "Process: " << process_name << " not found." << std::endl;
//