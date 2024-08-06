#include "SchedulerManager.h"
#include "ReadyAndFinished.h"
#include "CPUCore.h"
#include "FlatMemoryAllocator.h"
#include "PagingAllocator.h"
#include "ProcessScreen.h"
#include "OSConfig.h"
#include <string>
#include <iostream> 
#include <random>
#include <memory>

void commandCheck(std::string input, int pageCount);

int main() {
    OSConfig::initialize();
    OSConfig::readConfig();
    bool running = 1;
    int cpuCores = OSConfig::NUM_CPU;
    int quantumCycles = OSConfig::TIMESLICE;
    int lowerInstructionsBound = OSConfig::MIN_INS;
    int higherInstructionsBound = OSConfig::MAX_INS;
    float executionDelay = OSConfig::DELAYS_PER_EXEC;
    float creationDelay = OSConfig::BATCH_PROCESS_FREQ;

    int overallMemory = OSConfig::MAX_OVR_MEMORY;
    int processMemoryLower = OSConfig::MIN_MEM;
    int processMemoryHigher = OSConfig::MAX_MEM;
    int processMemory;
    int lowerPageCount = OSConfig::MIN_PAGE;
    int higherPageCount = OSConfig::MAX_PAGE;
    int pageCount;

    //Values for all above should be set by the text file in the config


    //Gets a random value from the lower and higher page count
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(lowerPageCount, higherPageCount);
    pageCount = dis(gen);

    //Sets a random memory size from the lower and higher memory (for paging allocator)
    int base = 2;
    std::uniform_int_distribution<> disMemory(processMemoryLower, processMemoryHigher);
    int exponent = disMemory(gen);
    processMemory = static_cast<int>(pow(base, exponent));

    //Gets the memory size of a frame from the memory and the page count
    int pageSize = processMemory / pageCount;


    //If the page count is more than one, it is a paging allocator
    if (pageCount == 1) {

        FlatMemoryAllocator::initialize(overallMemory);
        ReadyAndFinished::initialize();
        SchedulerManager::initialize(cpuCores, quantumCycles, creationDelay,
            executionDelay, lowerInstructionsBound, higherInstructionsBound,
            processMemoryLower, processMemoryHigher, pageCount);
    }
    else {

        PagingAllocator::initialize(overallMemory, pageSize, pageCount);
        ReadyAndFinished::initialize();
        SchedulerManager::initialize(cpuCores, quantumCycles, creationDelay,
            executionDelay, lowerInstructionsBound, higherInstructionsBound,
            processMemoryLower, processMemoryHigher, pageCount);
    }

    ProcessScreen::initialize();


    while (running) {

        std::cout << "Commands:" << std::endl;
        std::cout << "scheduler-test, scheduler-stop, screen-ls" << std::endl;
        std::cout << "report-util, exit, process-smi, vmstat" << std::endl;
        std::cout << "Enter command: ";
        std::string input;
        std::getline(std::cin, input);
        if (input == "exit") {
            running = 0;
        }
        else {
            commandCheck(input, pageCount);
        }

    }

};

void commandCheck(std::string input, int pCount) {



    if (input == "scheduler-test") {

        SchedulerManager::getInstance()->createProcesses();

    }
    else if (input == "scheduler-stop") {

        SchedulerManager::isCreatingProcesses = 0;

    }
    else if (input == "screen-ls") {

        CPUCore::isPrinting = 1;
        SchedulerManager::getInstance()->screenLS();
        CPUCore::isPrinting = 0;

    }
    else if (input == "report-util") {

        CPUCore::isPrinting = 1;
        SchedulerManager::getInstance()->reportUtil();
        CPUCore::isPrinting = 0;

    }
    else if (input == "process-smi") {

        //Should print from the respective allocator.
        CPUCore::isPrinting = 1;
        if (pCount > 1) {
            PagingAllocator::getInstance()->printProcessesInMemory();
        }
        else {
            FlatMemoryAllocator::getInstance()->printProcessesInMemory();
        }
        CPUCore::isPrinting = 0;

    }
    else if (input == "vmstat") {

        CPUCore::isPrinting = 1;

        //Should print from the respective allocator.
        if (pCount > 1) {
            PagingAllocator::getInstance()->vmStat();
        }
        else {
            FlatMemoryAllocator::getInstance()->vmStat();
        }
        CPUCore::isPrinting = 0;
    }

    /*
    else if (input == "screen-r processname") {

        CPUCore::isPrinting = 1;


        std::string processName = "ok"; //change to processName from the input
        std::shared_ptr<Process> process = nullptr;

        process = SchedulerManager::getInstance()->returnProcessInCore(processName);

        if (process == nullptr) {
            process = ReadyAndFinished::getInstance()->returnProcessInReady(processName);
        }

        if (process != nullptr) {
            ProcessScreen::getInstance()->run(process);
        }
        else {
            std::cout << "Process " << processName << " not found!" << std::endl;
        }

        CPUCore::isPrinting = 0;
    }*/

    else if (input.substr(0, 9) == "screen-r ") {//screen-r 

        CPUCore::isPrinting = 1;

        std::cout << "Reading process.." << std::endl;
        std::string processName = input.substr(9);
        std::shared_ptr<Process> process = nullptr;
        std::cout << "Searching for process " + processName << std::endl;
        process = SchedulerManager::getInstance()->returnProcessInCore(processName);

        if (process == nullptr) {
            process = ReadyAndFinished::getInstance()->returnProcessInReady(processName);
        }

        if (process != nullptr) {
            CPUCore::isPrinting = 0;
            ProcessScreen::getInstance()->run(process);
        }

        else {
            std::cout << "Process " << processName << " not found!" << std::endl;
        }
    }

    else if (input.substr(0, 9) == "screen-s ") {
        std::cout << "Creating process.." << std::endl;
        std::cout << "Process Name: " + input.substr(9) << std::endl;
        std::string processName = input.substr(9);
        std::shared_ptr<Process> process;
        process = SchedulerManager::getInstance()->createProcess(processName);
        CPUCore::isPrinting = 0;
        std::cout << "Process " + processName + " created..." << std::endl;
        ProcessScreen::getInstance()->run(process);
        std::cout << "Running process " + processName << std::endl;
    }

    else {

        std::cout << "Invalid input" << std::endl;

    }

};