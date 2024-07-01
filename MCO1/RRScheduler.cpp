#include "RRScheduler.h"
#include "CPUScheduler.h"
#include <thread>

void RRScheduler::runScheduler(){

  std::thread schedulerThread;

  schedulerThread = std::thread([this](){

    std::vector<std::shared_ptr<CPUCore>>& cpuCores = CPUScheduler::getInstance()->cpuCores;
    std::vector<std::shared_ptr<Process>>& readyQueue = CPUScheduler::getInstance()->readyQueue;

    while(isRunning){
      if((!(readyQueue.empty())) && (!(cpuCores.empty()))){
      
        for (auto& i : cpuCores){

          if(i->isAvailable){

            std::shared_ptr<Process> readyProcess = readyQueue.front();
            readyQueue.erase(readyQueue.begin());
            i->attachProcesstoCPUCore(readyProcess);

          }else{

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
