#include "FCFSScheduler.h"
#include "CPUCore.h"
#include "CPUScheduler.h"
#include <thread> 


void FCFSScheduler::runScheduler(){

  std::thread schedulerThread;
  schedulerThread = std::thread([this](){

    std::vector<std::shared_ptr<CPUCore>>& cpuCores = CPUScheduler::getInstance()->cpuCores;
    std::vector<std::shared_ptr<Process>>& readyQueue = CPUScheduler::getInstance()->readyQueue;

    while(isRunning){

      if((!(readyQueue.empty())) && !(cpuCores.empty())) {
      
          for (auto& i : cpuCores){

            if(i->isAvailable){

              std::shared_ptr<Process> readyProcess = readyQueue.front();
              readyQueue.erase(readyQueue.begin());
              i->attachProcesstoCPUCore(readyProcess);

            }
          }
      } 
    }

  });

  schedulerThread.detach();

};
