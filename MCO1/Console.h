#pragma once
#include <string>


class Console {

public:

	std::string consoleName;

	Console();

	Console(std::string consoleName);
	virtual ~Console(); 

	std::string getName();

	virtual void runConsole();		// Runs Console Process

	virtual void activate() = 0;
	virtual void display() = 0;
	virtual void process() = 0;

	friend class ConsoleManager;

private:

};