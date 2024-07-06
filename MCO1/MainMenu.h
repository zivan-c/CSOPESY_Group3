#pragma once
#include "Console.h"

class MainMenu : public Console
{
public:
	MainMenu();						// Constructor
	~MainMenu();					// Destructor
	void activate();
	void runConsole() override;

private:
	bool isNew = false;
	void help();					// Shows available instructions
	void setConfig();				// Set Configurations
	void printHeader();				// Show Banner
};