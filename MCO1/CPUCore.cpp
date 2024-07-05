#include "CPUCore.h"
#include "CPUScheduler.h"
#include "Process.h"
#include <chrono>
#include <thread>
#include <ctime>


CPUCore::CPUCore(int id, float executionDelay) {

    this->cpuCoreID = id;
    this->executionDelay = executionDelay;
    this->quantumCycles = 0;
    this->isAvailable = true;

};

CPUCore::CPUCore(int id, float executionDelay, int quantumCycles) {

    this->cpuCoreID = id;
    this->executionDelay = executionDelay;
    this->quantumCycles = quantumCycles;
    this->isAvailable = true;

};

void CPUCore::executeProcess() {
    if (processInCPUCore) {
        this->processInCPUCore->executeInstruction();
    }
};

std::shared_ptr<Process> CPUCore::getProcessinCPUCore() {

    if (processInCPUCore) {
        return processInCPUCore;
    }
    else {
        return nullptr;
    }

};


void CPUCore::runCore() {

    coreThread = std::thread([this]() {
        this->isRunning = true;

        //for checking
        std::cout << "CPUCORE: " << cpuCoreID << " is being run." << std::endl;

        while (isRunning) {
            //for checking, removing if processInCPUCore
            //if(processInCPUCore != nullptr){
            if (quantumCycles) {
                RRCPUBehavior();
            }
            else {
                normalCPUBehavior();
            }

            //}
        };
        });

    coreThread.detach();

};




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

void CPUCore::RRCPUBehavior() {
    if (processInCPUCore) {
        for (int i = 0; i < quantumCycles; i++) {
            if (processInCPUCore->getProcessState() == Process::ProcessState::PROCESSING) {
                executeProcess();
                std::chrono::duration<float, std::milli> delayDuration(executionDelay * 1000);
                std::this_thread::sleep_for(delayDuration);
            }
            else {
                // Lock the mutex before accessing the shared queue
                addToFinishedList();
                getProcessFromReadyQueue();
                break;
            }
        }

        // After processing, return the process to the ready queue
        returnProcesstoReadyQueue();
        getProcessFromReadyQueue();
    }
    else {
        // Lock the mutex before accessing the shared queue
        if (CPUScheduler::getInstance()->isReadyQueueAvailable()) {
            getProcessFromReadyQueue();
        }
    }
};


//FIX THIS
void CPUCore::attachProcesstoCPUCore() {

    if (this->isCoreFree()) {

        this->processInCPUCore = CPUScheduler::getInstance()->removeProcessFromReadyQueue();
        this->isAvailable = false;

        processInCPUCore->setCoreID(cpuCoreID);
        processInCPUCore->setProcessState(Process::ProcessState::PROCESSING);

        //for checking 
        std::cout << "process: " << this->processInCPUCore->getProcessName() << " has been attached to CPUCORE: "
            << this->cpuCoreID << std::endl;

    }

};


void CPUCore::getProcessFromReadyQueue() {

    if (CPUScheduler::getInstance()->isReadyQueueAvailable()) {

        std::lock_guard<std::mutex> lock(queueMutex); // Lock the mutex
        this->processInCPUCore = CPUScheduler::getInstance()->removeProcessFromReadyQueue();
        this->isAvailable = false;
        if (processInCPUCore) {

            processInCPUCore->setProcessState(Process::ProcessState::PROCESSING);
            std::cout << "Process: " << processInCPUCore->getProcessName() << " from ready queue sent to CPUCORE: "
                << cpuCoreID << std::endl;

        };


    }
    else {

    }

};


void CPUCore::addToFinishedList() {

    if (processInCPUCore) {
        processInCPUCore->setProcessState(Process::ProcessState::FINISHED);
        CPUScheduler::getInstance()->addProcessToFinishedProcesses(processInCPUCore);
        //for checking

        std::cout << "Process: " << processInCPUCore->getProcessName()
            << " sent to FINISHED LIST from CPUCORE: " << cpuCoreID << std::endl;
        removeProcessinCPUCore();
    }

};

void CPUCore::returnProcesstoReadyQueue() {

    std::lock_guard<std::mutex> lock(queueMutex);

    if (processInCPUCore) {
        processInCPUCore->setProcessState(Process::ProcessState::READY);
        CPUScheduler::getInstance()->addProcessToReadyQueue(processInCPUCore);
        //for checkign
        std::cout << "process: " << processInCPUCore->getProcessName() <<
            " returned to ready queue from CPUCORE: " << cpuCoreID << std::endl;
        removeProcessinCPUCore();
    }

};


void CPUCore::removeProcessinCPUCore() {


    if (processInCPUCore) {
        processInCPUCore.reset(); //Now, the CPUCore is empty;
        this->isAvailable = true; //CPU Core is available to be used
      //for checking
        std::cout << "CPUCORE: " << cpuCoreID << " is now empty." << std::endl;

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