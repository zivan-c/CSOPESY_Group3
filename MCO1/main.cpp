#include <iostream>
#include <memory>
#include <string>
#include "OSConfig.h"
#include "ConsoleManager.h"


int main() {
    try {
        //Initialize ConsoleManager
        ConsoleManager::initConMgr();

        // Get the ConsoleManager instance
        ConsoleManager* consoleMgr = ConsoleManager::getInst();

        // Main loop
        while (consoleMgr->isRunning()) {
            consoleMgr->process();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}