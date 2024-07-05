#include "FCFSScheduler.h"
#include "CPUScheduler.h"
#include <thread> 
#include "CPUCore.h"
#include "Process.h"


void FCFSScheduler::runScheduler() {

    std::vector<std::shared_ptr<CPUCore>>& reference = CPUScheduler::getInstance()->cpuCores;
    std::thread schedulerThread;
    schedulerThread = std::thread([this]() {

        while (isRunning) {

            if ((CPUScheduler::getInstance()->isReadyQueueAvailable()) && (!(CPUScheduler::getInstance()->cpuCores.empty()))) {
                for (auto i : CPUScheduler::getInstance()->cpuCores) {

                    if (i->isCoreFree()) {
                        i->getProcessFromReadyQueue();
                    }
                }
            }
        }

        });

    schedulerThread.detach();

};