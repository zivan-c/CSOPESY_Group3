#include "CPUCore.h"
#include "CPUScheduler.h"
#include "Process.h"
#include <chrono>
#include <thread>
#include <ctime>

//Constructor for CPUCore with the arguments and sets it to available
CPUCore::CPUCore(int id, float executionDelay, int quantumCycles) {

    this->cpuCoreID = id;
    this->executionDelay = executionDelay;
    this->quantumCycles = quantumCycles;
    this->isAvailable = true;

};

//Executes an instruction of the process inside
void CPUCore::executeProcess() {
    if (processInCPUCore) {
        this->processInCPUCore->executeInstruction();
    }
};

//Returns the pointer for the process inside of it
std::shared_ptr<Process> CPUCore::getProcessinCPUCore() {

    if (processInCPUCore) {
        return processInCPUCore;
    }
    else {
        return nullptr;
    }

};

//Method with an enclosed detached thread that is run concurrently alongside
//other cores
void CPUCore::runCore() {

    coreThread = std::thread([this]() {
        this->isRunning = true;

        //Different behaviors if RR is the scheduler
        while (isRunning) {
            if (quantumCycles) {
                RRCPUBehavior();
            }
            else {
                normalCPUBehavior();
            }
        };
        });

    coreThread.detach();

};


//Normal CPU Behavior, it is not in charge of getting new processes from the ready queue,
//the scheduler handles it
void CPUCore::normalCPUBehavior() {

    if (processInCPUCore) {
        if (processInCPUCore->getProcessState() == Process::ProcessState::PROCESSING) {
            executeProcess();
            std::chrono::duration<float, std::milli> delayDuration(executionDelay * 1000);
            std::this_thread::sleep_for(delayDuration);

        }
        else {
            addToFinishedList();
        }
    }
};

//RR scheduler CPU Behavior, where the core itself switches the processes from itself and the 
//ready queue, done so, since the quantum cycles of each core is not synchronized.
void CPUCore::RRCPUBehavior() {
    if (processInCPUCore) {
        for (int i = 0; i < quantumCycles; i++) {
            if (processInCPUCore->getProcessState() == Process::ProcessState::PROCESSING) {
                executeProcess();
                std::chrono::duration<float, std::milli> delayDuration(executionDelay * 1000);
                std::this_thread::sleep_for(delayDuration);
            }
            else {
                std::lock_guard<std::mutex> lock(queueMutex); // Lock the mutex
                addToFinishedList();
                getProcessFromReadyQueue();
                break;
            }
        }
        returnProcesstoReadyQueue();
        getProcessFromReadyQueue();
    }
    else {
        //std::lock_guard<std::mutex> lock(queueMutex); 
        if (CPUScheduler::getInstance()->isReadyQueueAvailable()) {
            getProcessFromReadyQueue();
        }
    }
};


//Gets a process from the readyqueue and assigns it to the core
void CPUCore::getProcessFromReadyQueue() {
    //
    std::lock_guard<std::mutex> lock(queueMutex);

    if (CPUScheduler::getInstance()->isReadyQueueAvailable()) {
        this->processInCPUCore = CPUScheduler::getInstance()->removeProcessFromReadyQueue();
        this->isAvailable = false;
        if (processInCPUCore) {
            processInCPUCore->setProcessState(Process::ProcessState::PROCESSING);
        };


    }
    else {

    }

};


//Adds the process to the finished list.
void CPUCore::addToFinishedList() {

    if (processInCPUCore) {
        processInCPUCore->setProcessState(Process::ProcessState::FINISHED);
        CPUScheduler::getInstance()->addProcessToFinishedProcesses(processInCPUCore);
        removeProcessinCPUCore();
    }

};

//Returns a process to the ready queue
//Used by the RR and Preemptive Scheduler
void CPUCore::returnProcesstoReadyQueue() {

    std::lock_guard<std::mutex> lock(queueMutex);
    if (processInCPUCore) {
        processInCPUCore->setProcessState(Process::ProcessState::READY);
        CPUScheduler::getInstance()->addProcessToReadyQueue(processInCPUCore);
        removeProcessinCPUCore();
    }

};


//Resets the process in the CPU core.
void CPUCore::removeProcessinCPUCore() {


    if (processInCPUCore) {
        processInCPUCore.reset(); //Now, the CPUCore is empty;
        this->isAvailable = true; //CPU Core is available to be used
      //for checking
    }
};

int CPUCore::getCoreID() {

    return this->cpuCoreID;

};

int CPUCore::isCoreFree() {

    std::lock_guard<std::mutex> lock(queueMutex); // Lock the mutex
    if (this->isAvailable) {
        return 1;
    }
    else {
        return 0;
    }
};