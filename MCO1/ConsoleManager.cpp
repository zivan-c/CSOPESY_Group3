#include "ConsoleManager.h"
#include "MainMenu.h"
#include "CPUScheduler.h"
#include "Scheduler.h"
#include "Process.h"
#include "ProcessScreen.h"
#include <iostream>
#include <unordered_map>
#include <stdexcept> // For std::runtime_error

ConsoleManager* ConsoleManager::cmmnInst = nullptr;

// Get the singleton instance of the ConsoleManager
ConsoleManager* ConsoleManager::getInst()
{
    if (cmmnInst == nullptr)
    {
        throw std::runtime_error("ConsoleManager instance has not initialized...");
    }
    return cmmnInst;
}

// Initialize the ConsoleManager
void ConsoleManager::initConMgr()
{
    if (cmmnInst == nullptr)
    {
        cmmnInst = new ConsoleManager();
    }
    else
    {
        throw std::runtime_error("ConsoleManager is already initialized...");
    }
}

// Destroy the ConsoleManager
void ConsoleManager::destroy()
{
    if (cmmnInst != nullptr)
    {
        delete cmmnInst;
        cmmnInst = nullptr;
    }
    else
    {
        throw std::runtime_error("ConsoleManager is not initialized.");
    }
}

// Draw the currently accessed console
void ConsoleManager::drawConsole() const
{
    if (this->curCon != nullptr)
    {
        this->curCon->display();
    }
    else
    {
        std::cerr << "No Console Assigned..." << std::endl;
    }
}

// Process the currently accessed console
void ConsoleManager::process() const
{
    if (this->curCon != nullptr)
    {
        this->curCon->process();
    }
    else
    {
        std::cerr << "No Console Assigned..." << std::endl;
    }
}

void ConsoleManager::changeConsole(std::string name)
{
    try
    {
        system("cls");
        this->lastCon = this->curCon;
        this->curCon = this->conMap.at(name);
        this->curCon->activate();
    }
    catch (const std::out_of_range&)
    {
        std::cerr << "Console " << name << " not found. Check screens or fix spelling..." << std::endl;
    }
}


// Unregister a screen from the conMap
void ConsoleManager::unregisterScreen(std::string name)
{
    if (this->conMap.erase(name) == 0)
    {
        std::cerr << "Screen " << name << " not found. Check screens or fix spelling..." << std::endl;
    }
}

// Change the currently accessed screen
void ConsoleManager::changeScreen(std::string name)
{
    try
    {
        system("cls");
        this->lastCon = this->curCon;
        this->curCon = this->conMap.at(name);
        this->curCon->activate();
    }
    catch (const std::out_of_range&)
    {
        std::cerr << "Screen " << name << " not found. Check screens or fix spelling..." << std::endl;
    }
}

// Constructor
ConsoleManager::ConsoleManager()
{
    this->isConsoleRunning = true;
    this->conHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Main Menu
    const std::shared_ptr<MainMenu> mainMenu = std::make_shared<MainMenu>();
    this->conMap[MAIN] = mainMenu;

    // Scheduler
    bool running = 1;
    int cpuCores = 4;
    int quantumCycles = 5;
    int lowerInstructionsBound = 10;
    int higherInstructionsBound = 100;
    float executionDelay = 0.25;
    int creationDelay = 2;
    int preemptive = 0;
    CPUScheduler::SchedulerAlgorithm schedulerAlgorithm = CPUScheduler::FCFS;
    CPUScheduler::initialize(cpuCores, schedulerAlgorithm, executionDelay, quantumCycles, preemptive, creationDelay, lowerInstructionsBound, higherInstructionsBound);

    // Marquee [Mich]
    // const std::shared_ptr<MarqueeConsole> marqueeConsole = std::make_shared<MarqueeConsole>();
    // this->consoleTable[MARQCONSOLE] = marqueeConsole;      

    // Screen [Almost Done]
    ProcessScreen::initialize();

    // Config

    this->changeConsole(MAIN);
}

// Check if the application is running
bool ConsoleManager::isRunning() const
{
    return this->isConsoleRunning;
}

// Exit the application
void ConsoleManager::exitApp()
{
    if (this->isConsoleRunning)
    {
        this->destroy();
    }
    else
    {
        std::cerr << "Application has stopped" << std::endl;
    }
}

// Get the console handle
HANDLE ConsoleManager::getConHandle() const
{
    return this->conHandle;
}

// Set the cursor position
void ConsoleManager::setCursorPosition(int posX, int posY) const
{
    COORD coord;
    coord.X = posX;
    coord.Y = posY;
    if (!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord)) {
        DWORD error = GetLastError();
        std::cerr << "Failed to set cursor position. Error code: " << error << std::endl;
    }
}