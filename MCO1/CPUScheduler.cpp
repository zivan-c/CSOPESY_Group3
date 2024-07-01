#include "CPUScheduler.h"
#include <chrono>
#include <thread>
#include <fstream>

CPUScheduler *CPUScheduler::singletonInstance = nullptr;
CPUScheduler *CPUScheduler::getInstance() { return singletonInstance; };

void CPUScheduler::initialize(int cpuCores,
                              CPUScheduler::SchedulerAlgorithm schedulerAlgorithm,
                              int executionDelay, int quantumCycles,
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
};

void CPUScheduler::setupCPUS() {

  if (this->scheduler == RR) {
    for (int i = 0; i < this->cpuCoresAmount; i++) {

      std::shared_ptr<CPUCore> cpuCore =
          std::make_shared<CPUCore>(i + 1, executionDelay);
      this->cpuCores.push_back(cpuCore);
    }
  } else {

    for (int i = 0; i < this->cpuCoresAmount; i++) {

      std::shared_ptr<CPUCore> cpuCore =
          std::make_shared<CPUCore>(i + 1, executionDelay, quantumCycles);
      this->cpuCores.push_back(cpuCore);
    }
  }
};

void CPUScheduler::setupScheduler() {

  if (this->scheduler == FCFS) {

    this->CPUSchedulerAlgorithm = FCFSScheduler();

  } else if (this->scheduler == RR) {

    this->CPUSchedulerAlgorithm = RRScheduler();

  } else {

    if (this->preemptive) {

      this->CPUSchedulerAlgorithm = PreemptiveSJF();

    } else {

      this->CPUSchedulerAlgorithm = NonPreemptiveSJF();
    }
  }
};

void CPUScheduler::createProcess(std::string name) {

  int isNameUsed = 0;
  for (auto &i : readyQueue) {

    if (i->getProcessName() == name) {
      std::cout << "The name " << name << " already exists.\n" << std::endl;
      isNameUsed = 1;
    }
  }

  if (!isNameUsed) {

    std::shared_ptr<Process> process = std::make_shared<Process>(
        name, this->instructionsLowerBound, this->instructionsHigherBound);
    Process::processCount++;
    this->readyQueue.push_back(process);

  } else {

    std::cout << "The name " << name << " already exists.\n" << std::endl;
  }
};

void CPUScheduler::createDynamicProcesses() {

  std::thread processCreationThread;

  processCreationThread = std::thread([this]() {
    // loop to create new processes and push them into the ready queue
    while (keepGenerating) {

      int isNameUsed = 0;
      std::string name = getNewProcessName();
      for (auto &i : readyQueue) {

        if (i->getProcessName() == name) {
          std::cout << "The name " << name << " already exists.\n" << std::endl;
          isNameUsed = 1;
        }
      }

      if (!isNameUsed) {
        std::shared_ptr<Process> process = std::make_shared<Process>(
            name, this->instructionsLowerBound, this->instructionsHigherBound);

        this->readyQueue.push_back(process);
        std::this_thread::sleep_for(std::chrono::seconds(this->creationDelay));
      } else {

        Process::processCount++;
      }
    };
  });

  processCreationThread.detach();
};

void CPUScheduler::stopGeneratingProcesses() { this->keepGenerating = false; };

std::string CPUScheduler::getNewProcessName() {

  std::string name = "p" + std::to_string(Process::processCount);
  Process::processCount++;
  return name;
}

void CPUScheduler::startScheduler() {

  for (auto &core : this->cpuCores) {
    core->runCore();
  }

  this->createDynamicProcesses();
};

void CPUScheduler::stopScheduler() {

  this->stopGeneratingProcesses();

  for (auto &core : this->cpuCores) {
    core->isRunning = false;
  }
};

void CPUScheduler::printReport() {

  std::cout << "CPU utilization: 100%\n" << std::endl;
  int coresUsed = 0;
  int coresAvailable = 0;
  for (auto &i : this->cpuCores) {
    if (i->isAvailable) {
      coresAvailable++;
    } else {
      i->getProcessinCPUCore()->setProcessState(Process::WAITING);
      coresUsed++;
    }
  }
  std::cout << "Cores used: " << coresUsed << std::endl;
  std::cout << "Cores available: " << coresAvailable << "\n" << std::endl;
  std::cout << "----------------------------------------" << "\n"
            << std::endl;

  std::cout << "Running processes:" << std::endl;

  for (auto &i : this->cpuCores) {
    if (!(i->isAvailable)) {

      std::string processName = i->getProcessinCPUCore()->getProcessName();
      std::string processInstructionTime = i->getProcessinCPUCore()->getDateAndTime();
      int coreID = i->getCoreID();
      int remainingInstructions = i->getProcessinCPUCore()->getRemainingInstructions();
      int totalInstructions = i->getProcessinCPUCore()->getTotalInstructions();

      std::cout << processName << " " << 
      processInstructionTime << " Core: " <<
      coreID << " " << 
      remainingInstructions << "/" <<
      totalInstructions << std::endl;


      
    }
  }

  std::cout << "\n";
  std::cout << "Finished processes:" << std::endl;

  for(auto& i : finishedProcesses){

      std::string processName = i->getProcessName();
      std::string processInstructionTime = i->getDateAndTime();
      Process::ProcessState processstate = i->getProcessState();
      int totalInstructions = i->getTotalInstructions();

      std::cout << processName << " " << 
      processInstructionTime << " " <<
      processstate << " " << 
      totalInstructions << "/" <<
      totalInstructions << std::endl;

  }

  std::cout << "----------------------------------------" << std::endl;

  for(auto& i : cpuCores){

    if(!(i->isAvailable))
    {

      i->getProcessinCPUCore()->setProcessState(Process::PROCESSING);

    }

  }

};

void CPUScheduler::createReportFile() {

  std::ofstream outputFile; 
  outputFile.open("csopesy-log.txt");

  if(!outputFile){
  
    std::cout << "File could not be opened." << std::endl;

  }else{


    outputFile << "CPU utilization: 100%\n" << std::endl;
    int coresUsed = 0;
    int coresAvailable = 0;
    for (auto &i : this->cpuCores) {
      if (i->isAvailable) {
        coresAvailable++;
      } else {
        i->getProcessinCPUCore()->setProcessState(Process::WAITING);
        coresUsed++;
      }
    }
    outputFile << "Cores used: " << coresUsed << std::endl;
    outputFile << "Cores available: " << coresAvailable << "\n" << std::endl;
    outputFile << "----------------------------------------" << "\n"
              << std::endl;

    outputFile << "Running processes:" << std::endl;

    for (auto &i : this->cpuCores) {
      if (!(i->isAvailable)) {

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

    for(auto& i : finishedProcesses){

        std::string processName = i->getProcessName();
        std::string processInstructionTime = i->getDateAndTime();
        Process::ProcessState processstate = i->getProcessState();
        int totalInstructions = i->getTotalInstructions();

        outputFile << processName << " " << 
        processInstructionTime << " " <<
        processstate << " " << 
        totalInstructions << "/" <<
        totalInstructions << std::endl;

    }

    outputFile << "----------------------------------------" << std::endl;
    outputFile.close();


    for(auto& i : cpuCores){

      if(!(i->isAvailable))
      {
        i->getProcessinCPUCore()->setProcessState(Process::PROCESSING);
      }
    }
    
    std::cout << "Report generated in csopesy-log.txt" << std::endl;
  }

};
