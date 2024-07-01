#include "PreemptiveSJF.h"
#include "CPUScheduler.h"



void PreemptiveSJF::runScheduler(){

  std::thread schedulerThread;

  schedulerThread = std::thread([this](){

    std::vector<std::shared_ptr<CPUCore>>& cpuCores = CPUScheduler::getInstance()->cpuCores;
    std::vector<std::shared_ptr<Process>>& readyQueue = CPUScheduler::getInstance()->readyQueue;

    while(isRunning){


      if((!(readyQueue.empty())) && (!(cpuCores.empty()))){
      
        for (auto& i : cpuCores){

          sortReadyQueue(readyQueue);

          if(i->isAvailable){

            std::shared_ptr<Process> readyProcess = readyQueue.front();
            readyQueue.erase(readyQueue.begin());
            i->attachProcesstoCPUCore(readyProcess);

          }else{

            i->getProcessinCPUCore()->setProcessState(Process::WAITING); //set to waiting to prevent instruction execution
            std::shared_ptr<Process> readyProcess = readyQueue.front();
            if((i->getProcessinCPUCore()->getRemainingInstructions()) < readyProcess->getRemainingInstructions()){

              i->returnProcesstoReadyQueue(readyQueue);
              i->attachProcesstoCPUCore(readyProcess);
              readyQueue.erase(readyQueue.begin());

            }
          }
        }
      } 
    }
  });


  schedulerThread.detach();

};



void PreemptiveSJF::sortReadyQueue(std::vector<std::shared_ptr<Process>>& readyQueue){



  //sort it according to the remaining number of instructions
  //the first one should be the one with the least number of instructions remaining


}; 

