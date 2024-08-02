#include "FlatMemoryAllocator.h"
#include "SchedulerManager.h"
#include <iostream>
#include <unordered_set>

//Defines the number of pages that go in and out,
//for flat memory a process constitutes to one page.
int FlatMemoryAllocator::numPagesIn = 0;
int FlatMemoryAllocator::numPagesOut = 0;

FlatMemoryAllocator* FlatMemoryAllocator::singletonInstance = nullptr;
FlatMemoryAllocator* FlatMemoryAllocator::getInstance() { return singletonInstance; };

//Constructor
FlatMemoryAllocator::FlatMemoryAllocator(int tMemory) :
    memory(tMemory, std::make_pair(nullptr, false)) {

    this->totalMemory = tMemory;
    this->usedMemory = 0;
    this->activeMemory = 0;
    this->inactiveMemory = 0;
    this->backingStore = "flatbacking_store.txt";

};

//Creates the singleton
void FlatMemoryAllocator::initialize(int tMemory) {
    singletonInstance = new FlatMemoryAllocator(tMemory);
};

//Main method to allocate a process
int FlatMemoryAllocator::allocateProcess(std::shared_ptr<Process> process) {

    std::lock_guard<std::mutex> lock(mtx);

    //Checks if the memory is too smalle for the process memory
    if ((process->processMemory) > totalMemory) {
        return 0;
    }

    if (isInMemory(process)) {
        return 1;
    }

    if (allocate(process)) {
        return 1;
    }

    //Checks non-active processes and places them in the backing store
    while (backingStoreOperation()) {

        std::shared_ptr<Process> processToRemove = nullptr;

        // Find the first process in the READY state
        for (const auto& block : memory) {
            const auto& process = block.first;
            bool occupied = block.second;

            if (occupied && (process->processState == Process::READY)) {
                processToRemove = process;
                break;
            }
        }

        // If a READY process is found, remove all its blocks

        if (processToRemove != nullptr) {

            process->serializeToFile(backingStore);

            for (auto& block : memory) {
                if (block.first == processToRemove) {
                    block.first = nullptr; // Clear the process pointer
                    block.second = false;   // Mark the block as free
                }
            }
            numPagesOut++;
        }
        else {
            return 0;
        }


        if (allocate(process)) {
            return 1;
        }
    }

    return 0;
};



//Method to deallocate a process from the main memory
void FlatMemoryAllocator::deallocateProcess(std::shared_ptr<Process> process) {

    std::lock_guard<std::mutex> lock(mtx);

    for (auto& block : memory) {
        if (block.first == process) {
            block.first = nullptr; // Clear the process pointer
            block.second = false;   // Mark the block as free
        }
    }
    numPagesOut++;

};

//Method that provides information on the active processes inside the memory
void FlatMemoryAllocator::printProcessesInMemory() {

    std::lock_guard<std::mutex> lock(mtx);

    int coresUsed = 0;
    for (auto& i : SchedulerManager::getInstance()->coreList) {
        if (!(i->isFree)) {
            coresUsed++;
        }
    }
    double CPUPercentage = ((double)coresUsed / (double)SchedulerManager::getInstance()->CPUCoreCount) * 100;
    int allocatedMemory = getActiveMemory();
    double memoryPercentage = ((double)allocatedMemory / (double)totalMemory) * 100;

    std::cout << "===============================================" << std::endl;
    std::cout << "\nCPU-Util: " << CPUPercentage << "%" << std::endl;
    std::cout << "Memory Usage: " << allocatedMemory << "KB / " << totalMemory << "KB" << std::endl;
    std::cout << "Memory Utilization: " << memoryPercentage << "%\n" << std::endl;
    std::cout << "Running processes and memory usage: \n" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    std::unordered_set<std::shared_ptr<Process> > processingProcesses;

    for (const auto& block : memory) {
        const auto& process = block.first;

        if (process && process->processState == Process::PROCESSING) {
            processingProcesses.insert(process);
        }
    }

    for (auto i : processingProcesses) {

        std::cout << i->processName << " " << i->processMemory << "KB" << std::endl;

    }

    std::cout << "-----------------------------------------------" << std::endl;
};


//Method that provides information on the memory
void FlatMemoryAllocator::vmStat() {

    std::lock_guard<std::mutex> lock(mtx);

    int externalFragmentation = getExternalFragmentation();
    int activeMemory = getActiveMemory();
    int usedMemory = activeMemory + externalFragmentation;

    std::cout << "\n" << totalMemory << "KB total memory" << std::endl;
    std::cout << usedMemory << "KB used memory" << std::endl;
    std::cout << activeMemory << "KB active memory" << std::endl;
    std::cout << externalFragmentation << "KB inactive memory" << std::endl;
    std::cout << CPUCore::idleTicks << " idle cpu ticks" << std::endl;
    std::cout << CPUCore::activeTicks << " active cpu ticks" << std::endl;
    std::cout << (CPUCore::activeTicks + CPUCore::idleTicks) << " total cpu ticks" << std::endl;
    std::cout << numPagesIn << " num paged in" << std::endl;
    std::cout << numPagesOut << " num paged out" << "\n" << std::endl;

};


//This method checks for the number of processes inside
//the main memory in a READY state, meaning, they are not
//currently active, which means they can be placed in the 
//backing store
int FlatMemoryAllocator::backingStoreOperation() {

    std::unordered_set<std::shared_ptr<Process> > readyProcesses;
    for (const auto& block : memory) {
        const auto& process = block.first;
        bool occupied = block.second;

        if (occupied && process->processState == Process::READY) {
            readyProcesses.insert(process);
        }
    }

    return readyProcesses.size();
}

//allocates a process inside the main memory
int FlatMemoryAllocator::allocate(std::shared_ptr<Process> process) {

    int freeCount = 0;
    int startIndex = 0;

    // Find a contiguous block of free memory
    for (int i = 0; i < memory.size(); ++i) {
        if (!memory[i].second) { // If the memory unit is free
            if (freeCount == 0) startIndex = i;
            freeCount++;
            if (freeCount == process->processMemory) break;
        }
        else {
            freeCount = 0;
        }
    }

    //If it encounters a free contiguous block
    if (freeCount >= process->processMemory) {
        for (int i = startIndex; i < startIndex + process->processMemory; ++i) {
            memory[i] = std::make_pair(process, true);
        }
        numPagesIn++;
        process->processState = Process::PROCESSING;
        return 1;
    }
    else {
        return 0;
    }

};

//Gets all the free blocks in between the first
//and the last process inside the main memory
int FlatMemoryAllocator::getExternalFragmentation() {

    int firstAllocatedIndex = memory.size();
    int lastAllocatedIndex = 0;

    // Identify the first and last allocated blocks
    for (int i = 0; i < memory.size(); ++i) {
        if (memory[i].first != nullptr) {
            if (i < firstAllocatedIndex) firstAllocatedIndex = i;
            if (i > lastAllocatedIndex) lastAllocatedIndex = i;
        }
    }

    // If no blocks are allocated, return 0
    if (firstAllocatedIndex == memory.size()) {
        return 0;
    }

    // Count free blocks between the first and last allocated blocks
    int freeBlocks = 0;
    for (int i = firstAllocatedIndex; i <= lastAllocatedIndex; ++i) {
        if (memory[i].second == false) {
            freeBlocks++;
        }
    }

    return freeBlocks;
};


//Gets all the memory blocks that are occupied to symbolize the active memory
int FlatMemoryAllocator::getActiveMemory() {

    int activeMemory = 0;
    for (int i = 0; i < memory.size(); ++i) {
        if (memory[i].first != nullptr) {
            activeMemory++;
        }
    }

    return activeMemory;
};


//Method checks if the process trying to be allocated already resides in the main memory,
//if it is, just set the process state to PROCESING and return true
int FlatMemoryAllocator::isInMemory(std::shared_ptr<Process> process) {

    for (int i = 0; i < memory.size(); ++i) {
        if (memory[i].first == process) {
            process->processState = Process::PROCESSING;
            return 1;
        }
    }
    return 0;
};