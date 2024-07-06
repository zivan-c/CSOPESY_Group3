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


    //Sets up the default values for process for subsequent process creation.
    Process::processIDCount = 0;
    Process::processCount = 0;


    //Sets up the singleton member with arguments passed from config.txt file. 
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

    //Starts threads for CPU cores
    for (auto& core : singletonInstance->cpuCores) {
        core->runCore();
    }

    //Starts the thread for the scheduler
    singletonInstance->CPUSchedulerAlgorithm->runScheduler();


};

void CPUScheduler::setupCPUS() {

    //If the scheduler is RR create the cores with the set quantum cycles
    if (this->scheduler == RR) {
        for (int i = 0; i < this->cpuCoresAmount; i++) {
            std::shared_ptr<CPUCore> cpuCore =
                std::make_shared<CPUCore>(i + 1, executionDelay, quantumCycles);

            this->cpuCores.push_back(cpuCore);

        }
    }
    else {

        for (int i = 0; i < this->cpuCoresAmount; i++) {

            std::shared_ptr<CPUCore> cpuCore =
                std::make_shared<CPUCore>(i + 1, executionDelay, 0);
            this->cpuCores.push_back(cpuCore);
        }
    }
};

//Assigns the scheduler to be run according to what was passed to the initialize() method.
void CPUScheduler::setupScheduler() {

    if (this->scheduler == FCFS) {

        this->CPUSchedulerAlgorithm = std::make_shared<FCFSScheduler>();

    }
    else if (this->scheduler == RR) {

        this->CPUSchedulerAlgorithm = std::make_shared<RRScheduler>();

    }
    else {

        if (this->preemptive) {

            this->CPUSchedulerAlgorithm = std::make_shared<PreemptiveSJF>();

        }
        else {

            this->CPUSchedulerAlgorithm = std::make_shared<NonPreemptiveSJF>();
        }
    }


};

//For the manual creation of a process
void CPUScheduler::createProcess(std::string name) {

    int isNameUsed = 0;

    //Checks if the process is already present somewhere
    if (!(readyQueue.empty())) {
        for (auto& i : readyQueue) {

            if (i->getProcessName() == name) {
                isNameUsed = 1;
            }
        }
    }

    //If not used, then create it and push to the vector
    if (!isNameUsed) {

        std::shared_ptr<Process> process = std::make_shared<Process>(
            name, this->instructionsLowerBound, this->instructionsHigherBound);
        //for checking of process creation
        Process::processCount++;
        this->pushToReadyQueue(process);


        if (scheduler == SJF) {
            this->sortReadyQueue();
        };

    }
    else {

        std::cout << "The name " << name << " already exists." << std::endl;
    }
};

//For the scheduler-test command, which creates processes and pushes them to the ready queue
void CPUScheduler::createDynamicProcesses() {

    std::thread processCreationThread;

    //Enclosed inside a thread that runs detached.
    processCreationThread = std::thread([this]() {
        // loop to create new processes and push them into the ready queue
        while (keepGenerating) {

            //int isNameUsed = 0;
            std::string name = getNewProcessName();

            //if(!(readyQueue.empty())){

              //for (auto &i : readyQueue) {

                //if (i->getProcessName() == name) {
                  //std::cout << "The name " << name << " already exists.\n" << std::endl;
                  //isNameUsed = 1;
                //}
              //}
           // }

            //if (!isNameUsed) {
            std::shared_ptr<Process> process = std::make_shared<Process>(
                name, this->instructionsLowerBound, this->instructionsHigherBound);
            //for checking of process creation

            this->pushToReadyQueue(process);
            //
                    //If the scheduler is SJF, sort the readyQueue with the addition of the new process
            if (scheduler == SJF) {

                this->sortReadyQueue();

            }

            //Delay for creation of new process
            std::chrono::duration<float, std::milli> delayDuration(creationDelay * 1000);
            std::this_thread::sleep_for(delayDuration);

            //} else {

              //In the case that, say, Process_13 was created manually, so it still increments
              //Process::processCount++;
            //}
        };

        });

    processCreationThread.detach();
};


//Creates a new process name with the number of processes created already
std::string CPUScheduler::getNewProcessName() {

    std::string name = "Process_" + std::to_string(Process::processCount);
    Process::processCount++;
    return name;
};

//Method that starts the creation of processes
void CPUScheduler::startScheduler() {
    //for checking
    std::cout << "Started process creation" << std::endl;
    this->keepGenerating = true;
    this->createDynamicProcesses();
};

//Method for scheduler-stop that stops the creation of new processes.
void CPUScheduler::stopScheduler() {

    this->keepGenerating = false;
    std::cout << "Stopped process creation" << std::endl;

};

//Method for screen-ls
void CPUScheduler::printReport() {

    //To check how many cores are being used
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

    //For utilization percentage
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

                //Prints the process information inside the core.
                std::string processName = process->getProcessName();
                std::string processInstructionTime = process->getInstructionTime();
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

    //Prints out process information for all finished processes
    for (auto i : finishedProcesses) {

        std::string processName = i->getProcessName();
        std::string processInstructionTime = i->getInstructionTime();
        int totalInstructions = i->getTotalInstructions();

        std::cout << processName << " " <<
            processInstructionTime << " FINISHED " <<
            totalInstructions << "/" <<
            totalInstructions << std::endl;

    }

    std::cout << "----------------------------------------" << std::endl;

    for (auto& i : this->cpuCores) {
        if (i->isCoreFree()) {
            i->getProcessinCPUCore()->setProcessState(Process::ProcessState::PROCESSING);
        }
    }

};

//Similar to printReport() but outputs to a text file. 
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
                std::string processInstructionTime = i->getProcessinCPUCore()->getInstructionTime();
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
            std::string processInstructionTime = i->getInstructionTime();
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


//Returns the remaining instructions of the process at the front of the sorted vector
//in ascending order due to SJF
int CPUScheduler::returnLowestRemainingInstructions() {

    auto front = readyQueue.front();
    int lowestInstructions = front->getRemainingInstructions();
    return lowestInstructions;
}


//Returns the process at the front of the ready queue
//Uses mutexes to avoid race conditions
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

//For returning a process to the ready queue from the core
//for algorithms: RR and Preemptive SJF
void CPUScheduler::addProcessToReadyQueue(std::shared_ptr<Process> process) {
    this->pushToReadyQueue(process);
};

//Checks if the readyQueue available
//Also enclosed in a mutex
bool CPUScheduler::isReadyQueueAvailable() {

    std::lock_guard<std::mutex> lock(queueMutex); // Lock the mutex
    if (readyQueue.empty()) {
        return false;
    }
    else {
        return true;
    }

};

//Adds a process to the finished processes
void CPUScheduler::addProcessToFinishedProcesses(std::shared_ptr<Process> process) {

    std::lock_guard<std::mutex> lock(queueMutex); // Lock the mutex
    finishedProcesses.push_back(process);

};


//Sorts the ready queue for the SJF algorithms in ascending order of their remaining instructions
//Enclosed in a mutex to avoid race conditions
void CPUScheduler::sortReadyQueue() {

    std::lock_guard<std::mutex> lock(queueMutex); // Lock the mutex
    //
    std::sort(readyQueue.begin(), readyQueue.end(), [](const std::shared_ptr<Process>& a, const std::shared_ptr<Process>& b) {
        return a->getRemainingInstructions() < b->getRemainingInstructions();
        });
};


//Returns a pointer to the process to use for screen-s and screen-r
//It checks for the process in both the CPU cores and the ready queue
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



void CPUScheduler::pushToReadyQueue(std::shared_ptr<Process> process) {

    std::lock_guard<std::mutex> lock(queueMutex); // Lock the mutex
    readyQueue.push_back(process);


};