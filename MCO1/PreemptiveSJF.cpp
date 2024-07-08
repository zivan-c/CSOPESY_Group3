#include "PreemptiveSJF.h"
#include "CPUScheduler.h"



void PreemptiveSJF::runScheduler(){

  std::thread schedulerThread;

  schedulerThread = std::thread([this](){

    while(isRunning){

      //Checks if ready queue has processes and there are CPU cores)
      if((CPUScheduler::getInstance()->getReadyQueue()->isReadyQueueAvailable()) && (!(CPUScheduler::getInstance()->cpuCores.empty()))) {
      
        for (auto& i : CPUScheduler::getInstance()->cpuCores){

          //Moves a process from the ready queue to the CPU core if available
          if(i->isCoreFree()){
            i->getProcessFromReadyQueue();
          }else{
              i->getProcessinCPUCore()->setProcessState(Process::ProcessState::WAITING); //set to waiting to prevent instruction execution
            //if the instructions left in the process in the core is greater than the one
            //in the front of th sorted ready queue
              if((i->getProcessinCPUCore()->getRemainingInstructions()) >
            CPUScheduler::getInstance()->getReadyQueue()->returnLowestRemainingInstructions()){

                //Exchange of processes
                i->returnProcesstoReadyQueue();
                i->getProcessFromReadyQueue();

              }else{

                //If less, continue to execute
                i->getProcessinCPUCore()->setProcessState(Process::ProcessState::PROCESSING);
              }

          }
        }
      } 
    }
  });


  schedulerThread.detach();

};


