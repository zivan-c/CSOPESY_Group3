#pragma once
#include "Console.h"

class MainMenu : public Console
{
public:
	MainMenu();						// Constructor
	~MainMenu();					// Destructor

	// Overridden Console Methods
	void activate() override;		 
	void display() override;		 
	void process() override;		 

private:
	bool isNew = false;
	void help();					// Shows available instructions
	void setConfig();				// Set Configurations
	void printHeader();				// Show Banner
};