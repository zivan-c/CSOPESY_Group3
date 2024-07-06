#include "CPUScheduler.h"
#include "CPUCore.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"
#include "NonPreemptiveSJF.h"
#include "Process.h"
#include "PreemptiveSJF.h"
#include <chrono>
#include <thread>
#include <fstream>
#include <algorithm>

CPUScheduler* CPUScheduler::singletonInstance = nullptr;
CPUScheduler* CPUScheduler::getInstance() { return singletonInstance; };



void CPUScheduler::initialize(int cpuCores,
    SchedulerAlgorithm schedulerAlgorithm,
    float executionDelay, int quantumCycles,
    int preemptive, int creationDelay,
    int instructionsLowerBound,
    int instructionsHigherBound) {

    Process::processIDCount = 0;
    Process::processCount = 0;
    singletonInstance = new CPUScheduler();

    singletonInstance->cpuCoresAmount = cpuCores;
    singletonInstance->scheduler = schedulerAlgorithm;
    singletonInstance->quantumCycles = quantumCycles;
    singletonInstance->preemptive = preemptive;
    singletonInstance->creationDelay = creationDelay;
    singletonInstance->instructionsLowerBound = instructionsLowerBound;
    singletonInstance->instructionsHigherBound = instructionsHigherBound;
    singletonInstance->executionDelay = executionDelay;

    singletonInstance->setupCPUS();
    singletonInstance->setupScheduler();

    //removed & for testing, will report back if it does anything
    for (auto core : singletonInstance->cpuCores) {
        core->runCore();
    }

    singletonInstance->CPUSchedulerAlgorithm->runScheduler();


};

void CPUScheduler::setupCPUS() {

    if (this->scheduler == RR) {
        for (int i = 0; i < this->cpuCoresAmount; i++) {
            std::shared_ptr<CPUCore> cpuCore =
                std::make_shared<CPUCore>(i + 1, executionDelay, quantumCycles);
            //for checking
            std::cout << "Created CPUCORE: " << i + 1 << std::endl;

            this->cpuCores.push_back(cpuCore);

            //for checking
            std::cout << "Pushed CPUCORE: " << i + 1 << " to main CPUScheduler Vector" << std::endl;
        }
    }
    else {

        for (int i = 0; i < this->cpuCoresAmount; i++) {

            std::shared_ptr<CPUCore> cpuCore =
                std::make_shared<CPUCore>(i + 1, executionDelay, 0);
            //for checking
            std::cout << "Created CPUCORE: " << i + 1 << std::endl;
            this->cpuCores.push_back(cpuCore);

            //for checking
            std::cout << "Pushed CPUCORE: " << i + 1 << " to main CPUScheduler Vector" << std::endl;

        }
    }
};

void CPUScheduler::setupScheduler() {

    if (this->scheduler == FCFS) {

        this->CPUSchedulerAlgorithm = std::make_shared<FCFSScheduler>();
        //checking
        std::cout << "FCFSScheduler() has been assigned to the scheduler" << std::endl;

    }
    else if (this->scheduler == RR) {

        this->CPUSchedulerAlgorithm = std::make_shared<RRScheduler>();
        //checking
        std::cout << "RRScheduler() has been assigned to the scheduler" << std::endl;

    }
    else {

        if (this->preemptive) {

            this->CPUSchedulerAlgorithm = std::make_shared<PreemptiveSJF>();
            std::cout << "PreemptiveSJF() has been assigned to the scheduler" << std::endl;

        }
        else {

            this->CPUSchedulerAlgorithm = std::make_shared<NonPreemptiveSJF>();
            std::cout << "PreemptiveSJF() has been assigned to the scheduler" << std::endl;
        }
    }


};

void CPUScheduler::createProcess(std::string name) {

    int isNameUsed = 0;

    if (!(readyQueue.empty())) {
        for (auto& i : readyQueue) {

            if (i->getProcessName() == name) {
                isNameUsed = 1;
            }
        }
    }

    if (!isNameUsed) {

        std::shared_ptr<Process> process = std::make_shared<Process>(
            name, this->instructionsLowerBound, this->instructionsHigherBound);
        //for checking of process creation
        std::cout << "Process: " << name << " created" << std::endl;
        Process::processCount++;
        this->readyQueue.push_back(process);

        if (scheduler == SJF) {
            this->sortReadyQueue();
        };

        //for checking of pushing to ready queue
        std::cout << "Process: " << name << " pushed to ready queue" << std::endl;

    }
    else {

        std::cout << "The name " << name << " already exists." << std::endl;
    }
};

void CPUScheduler::createDynamicProcesses() {

    std::thread processCreationThread;

    processCreationThread = std::thread([this]() {
        // loop to create new processes and push them into the ready queue
        while (keepGenerating) {

            int isNameUsed = 0;
            std::string name = getNewProcessName();

            if (!(readyQueue.empty())) {

                for (auto& i : readyQueue) {

                    if (i->getProcessName() == name) {
                        std::cout << "The name " << name << " already exists.\n" << std::endl;
                        isNameUsed = 1;
                    }
                }
            }

            if (!isNameUsed) {
                std::shared_ptr<Process> process = std::make_shared<Process>(
                    name, this->instructionsLowerBound, this->instructionsHigherBound);
                //for checking of process creation

                this->readyQueue.push_back(process);

                if (scheduler == SJF) {

                    this->sortReadyQueue();

                }

                //for checking of pushing to ready queue
                //
                std::chrono::duration<float, std::milli> delayDuration(creationDelay * 1000);
                std::this_thread::sleep_for(delayDuration);

            }
            else {

                Process::processCount++;
            }
        };

        });

    processCreationThread.detach();
};


std::string CPUScheduler::getNewProcessName() {

    std::string name = "Process_" + std::to_string(Process::processCount);
    Process::processCount++;
    return name;
};

void CPUScheduler::startScheduler() {
    //for checking
    std::cout << "Started process creation" << std::endl;
    this->keepGenerating = true;
    this->createDynamicProcesses();
};

void CPUScheduler::stopScheduler() {

    this->keepGenerating = false;
    std::cout << "Stopped process creation" << std::endl;

    //for checking
    for (auto i : this->readyQueue) {

        std::cout << i->getProcessName() << " ";
        std::cout << i->getRemainingInstructions() << "/";
        std::cout << i->getTotalInstructions() << std::endl;

    }

    for (auto i : this->finishedProcesses) {

        std::cout << "Finished Process: " << i->getProcessName() << std::endl;

    }


};

void CPUScheduler::printReport() {

    int coresUsed = 0;
    int coresAvailable = 0;
    for (auto& i : this->cpuCores) {
        if (i->isCoreFree()) {
            coresAvailable++;
        }
        else {
            coresUsed++;
        }
    }

    double percentage = ((double)coresUsed / (double)cpuCoresAmount) * 100;

    std::cout << "CPU utilization: " << percentage << "%\n" << std::endl;
    std::cout << "Cores used: " << coresUsed << std::endl;
    std::cout << "Cores available: " << coresAvailable << "\n" << std::endl;
    std::cout << "----------------------------------------" << "\n"
        << std::endl;

    std::cout << "Running processes:" << std::endl;

    for (auto i : this->cpuCores) {
        if (i != nullptr && !(i->isCoreFree())) {  // Check if i is not a null pointer and if it's not available

            auto process = i->getProcessinCPUCore();
            if (process != nullptr) {  // Check if the process is not a null pointer
                std::string processName = process->getProcessName();
                std::string processInstructionTime = process->getDateAndTime();
                int coreID = i->getCoreID();
                int remainingInstructions = process->getRemainingInstructions();
                int totalInstructions = process->getTotalInstructions();

                std::cout << processName << " " <<
                    processInstructionTime << " Core: " <<
                    coreID << " " <<
                    remainingInstructions << "/" <<
                    totalInstructions << std::endl;
            }
            else {
                std::cerr << "Error: getProcessinCPUCore() returned a null pointer" << std::endl;
            }
        }
        else {
            if (i == nullptr) {
                std::cerr << "Error: Encountered null pointer in cpuCores" << std::endl;
            }
        }
    }



    std::cout << "\n";
    std::cout << "Finished processes:" << std::endl;

    //removed & in auto
    for (auto i : finishedProcesses) {

        std::string processName = i->getProcessName();
        std::string processInstructionTime = i->getDateAndTime();
        int totalInstructions = i->getTotalInstructions();

        std::cout << processName << " " <<
            processInstructionTime << " FINISHED " <<
            totalInstructions << "/" <<
            totalInstructions << std::endl;

    }

    std::cout << "----------------------------------------" << std::endl;

};

void CPUScheduler::createReportFile() {

    std::ofstream outputFile;
    outputFile.open("csopesy-log.txt");

    if (!outputFile) {

        std::cout << "File could not be opened." << std::endl;

    }
    else {

        int coresUsed = 0;
        int coresAvailable = 0;
        for (auto& i : this->cpuCores) {
            if (i->isCoreFree()) {
                coresAvailable++;
            }
            else {
                coresUsed++;
            }
        }

        double percentage = ((double)coresUsed / (double)cpuCoresAmount) * 100;

        outputFile << "CPU utilization: " << percentage << "%\n" << std::endl;
        outputFile << "Cores used: " << coresUsed << std::endl;
        outputFile << "Cores available: " << coresAvailable << "\n" << std::endl;
        outputFile << "----------------------------------------" << "\n"
            << std::endl;

        outputFile << "Running processes:" << std::endl;

        for (auto& i : this->cpuCores) {
            if (!(i->isCoreFree())) {

                std::string processName = i->getProcessinCPUCore()->getProcessName();
                std::string processInstructionTime = i->getProcessinCPUCore()->getDateAndTime();
                int coreID = i->getCoreID();
                int remainingInstructions = i->getProcessinCPUCore()->getRemainingInstructions();
                int totalInstructions = i->getProcessinCPUCore()->getTotalInstructions();

                outputFile << processName << " " <<
                    processInstructionTime << " Core: " <<
                    coreID << " " <<
                    remainingInstructions << "/" <<
                    totalInstructions << std::endl;

            }
        }

        outputFile << "\n";
        outputFile << "Finished processes:" << std::endl;

        for (auto& i : finishedProcesses) {

            std::string processName = i->getProcessName();
            std::string processInstructionTime = i->getDateAndTime();
            int totalInstructions = i->getTotalInstructions();

            outputFile << processName << " " <<
                processInstructionTime << " FINISHED " <<
                totalInstructions << "/" <<
                totalInstructions << std::endl;

        }

        outputFile << "----------------------------------------" << std::endl;
        outputFile.close();

        std::cout << "Report generated in csopesy-log.txt" << std::endl;
    }

};


int CPUScheduler::returnLowestRemainingInstructions() {

    auto front = readyQueue.front();
    int lowestInstructions = front->getRemainingInstructions();
    return lowestInstructions;
}


std::shared_ptr<Process> CPUScheduler::removeProcessFromReadyQueue() {

    std::lock_guard<std::mutex> lock(queueMutex); // Lock the mutexcpusch.c
    if (!readyQueue.empty()) {
        auto front = readyQueue.front();
        readyQueue.erase(readyQueue.begin());
        return front;
    }
    else {
        return nullptr; // or throw an exception, handle as appropriate
    }

};

void CPUScheduler::addProcessToReadyQueue(std::shared_ptr<Process> process) {

    std::lock_guard<std::mutex> lock(queueMutex); // Lock the mutex
    readyQueue.push_back(process);

};

bool CPUScheduler::isReadyQueueAvailable() {

    std::lock_guard<std::mutex> lock(queueMutex); // Lock the mutex
    if (readyQueue.empty()) {
        return false;
    }
    else {
        return true;
    }

};

void CPUScheduler::addProcessToFinishedProcesses(std::shared_ptr<Process> process) {

    std::lock_guard<std::mutex> lock(queueMutex); // Lock the mutex
    finishedProcesses.push_back(process);

};


void CPUScheduler::sortReadyQueue() {

    std::lock_guard<std::mutex> lock(queueMutex); // Lock the mutex
    //
    std::sort(readyQueue.begin(), readyQueue.end(), [](const std::shared_ptr<Process>& a, const std::shared_ptr<Process>& b) {
        return a->getRemainingInstructions() < b->getRemainingInstructions();
        });
};


std::shared_ptr<Process> CPUScheduler::getProcessPointer(std::string process) {

    for (auto& i : cpuCores) {

        if (!(i->isCoreFree())) {
            if (i->getProcessinCPUCore()->getProcessName() == process) {
                return i->getProcessinCPUCore();
            }
        }
    }

    if (!(readyQueue.empty())) {
        for (auto& i : readyQueue) {
            if (i->getProcessName() == process) {
                return i;
            }
        }
    }

    return nullptr;


};