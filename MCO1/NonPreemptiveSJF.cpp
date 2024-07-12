#include "NonPreemptiveSJF.h"
#include "CPUScheduler.h"
#include "ReadyQueue.h"
#include <thread>

void NonPreemptiveSJF::runScheduler() {

  std::thread schedulerThread;

  schedulerThread = std::thread([this]() {

    while (isRunning) {

      if ((ReadyQueue::getInstance()->isReadyQueueAvailable()) &&
          (!(CPUScheduler::getInstance()->cpuCores.empty()))) {

        for (auto i : CPUScheduler::getInstance()->cpuCores) {

          //Very similar to FCFS, but ready queue is sorted according to remaining instructions left
          if (i->isCoreFree()) {
            i->getProcessFromReadyQueue();
          }
        }
      }
    }
  });

  schedulerThread.detach();
};
