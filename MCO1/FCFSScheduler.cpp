#include "FCFSScheduler.h"
#include "CPUScheduler.h"
#include "ReadyQueue.h"
#include <thread> 


void FCFSScheduler::runScheduler() {

    std::thread schedulerThread;
    schedulerThread = std::thread([this]() {

        while (isRunning) {

            if ((ReadyQueue::getInstance()->isReadyQueueAvailable()) && (!(CPUScheduler::getInstance()->cpuCores.empty()))) {
                for (auto i : CPUScheduler::getInstance()->cpuCores) {

                    //Checks if the core is free, then attaches a process from the ready queue to it
                    if (i->isCoreFree()) {
                        i->getProcessFromReadyQueue();
                    }
                }
            }
        }

        });

    schedulerThread.detach();

};