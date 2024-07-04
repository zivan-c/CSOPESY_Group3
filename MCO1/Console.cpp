#pragma once
#include <string>
#include "Console.h"

Console::Console(std::string name) {
    this->consoleName = name;
}

std::string Console::getName() {
    return this->consoleName;
}

Console::~Console() { // Implement the destructor
}