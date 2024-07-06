#pragma once
#include <string>
#include "Console.h"

Console::Console() {
    // Initialize any necessary members
}

Console::Console(std::string name) {
    this->consoleName = name;
}

std::string Console::getName() {
    return this->consoleName;
}

Console::~Console() { // Implement the destructor
}

// Define the runConsole method
void Console::runConsole() {
    // Implementation of runConsole
}

// Define the cmdReader method