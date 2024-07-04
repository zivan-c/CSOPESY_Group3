#include "FCFSScheduler.h"
#include "CPUScheduler.h"
#include <thread> 
#include "CPUCore.h"
#include "Process.h"


void FCFSScheduler::runScheduler() {

    std::thread schedulerThread;
    schedulerThread = std::thread([this]() {

        while (isRunning) {

            if ((CPUScheduler::getInstance()->isReadyQueueAvailable()) && (!(CPUScheduler::getInstance()->cpuCores.empty()))) {

                for (auto& i : CPUScheduler::getInstance()->cpuCores) {
                    if (i->isCoreFree()) {
                        //std::cout << "RR Checking CPUCORE: " << i->getCoreID() << std::endl;
                        i->getProcessFromReadyQueue();
                    }
                }
            }
        }

        });

    schedulerThread.detach();

};