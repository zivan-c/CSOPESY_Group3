#pragma once
#include <string>

class Console{
private:
	std::string consoleName;

public:
	Console(std::string consoleName);
	~Console(); 

	std::string getName();

	virtual void runConsole();		// Runs Console Process
	virtual void cmdReader();			// Reads Commands


	virtual void activate() = 0;
	virtual void display() = 0;
	virtual void process() = 0;

	friend class ConsoleManager;
};
