#include "NonPreemptiveSJF.h"
#include "CPUScheduler.h"
#include <algorithm>
#include <thread>


void NonPreemptiveSJF::runScheduler(){
  
  
  std::thread schedulerThread;

  schedulerThread = std::thread([this](){

    std::vector<std::shared_ptr<CPUCore>>& cpuCores = CPUScheduler::getInstance()->cpuCores;
    std::vector<std::shared_ptr<Process>>& readyQueue = CPUScheduler::getInstance()->readyQueue;

    while(isRunning){

      if((!(readyQueue.empty())) && (!(cpuCores.empty()))) {
      
        for (auto& i : cpuCores){

          sortReadyQueue(readyQueue);

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



void NonPreemptiveSJF::sortReadyQueue(std::vector<std::shared_ptr<Process>>& readyQueue){


  //sort it according to the remaining number of instructions
  //the process at the front is the process with the least amount of remaining instructions
  std::sort(readyQueue.begin(), readyQueue.end(),
            [](const std::shared_ptr<Process>& p1, const std::shared_ptr<Process>& p2) {
              return p1->getRemainingInstructions() < p2->getRemainingInstructions();
            });
  
     


}; 



