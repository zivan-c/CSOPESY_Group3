#include "NonPreemptiveSJF.h"
#include "CPUScheduler.h"
#include <thread>


void NonPreemptiveSJF::runScheduler() {


    std::thread schedulerThread;

    schedulerThread = std::thread([this]() {

        std::vector<std::shared_ptr<CPUCore>>& cpuCores = CPUScheduler::getInstance()->cpuCores;

        while (isRunning) {

            if ((!(CPUScheduler::getInstance()->isReadyQueueAvailable())) && (!(cpuCores.empty()))) {

                for (auto& i : cpuCores) {

                    CPUScheduler::getInstance()->sortReadyQueue();

                    if (i->isCoreFree()) {

                        i->attachProcesstoCPUCore();

                    }
                }
            }
        }
        });

    schedulerThread.detach();


};