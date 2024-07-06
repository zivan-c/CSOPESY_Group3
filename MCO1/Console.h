#pragma once
#include <string>


class Console {

public:
	Console();
	~Console();
	virtual void runConsole();		// Runs Console Process

private:
	std::string conName;
};