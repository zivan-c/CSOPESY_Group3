#include "RRScheduler.h"
#include "CPUScheduler.h"
#include <thread>

void RRScheduler::runScheduler() {

    std::thread schedulerThread;
    isRunning = 1;

    schedulerThread = std::thread([this]() {

        std::vector<std::shared_ptr<CPUCore>>& cpuCores = CPUScheduler::getInstance()->cpuCores;

        std::cout << "RRSCHED is running!" << std::endl;
        /**/
        while (isRunning) {
            if ((CPUScheduler::getInstance()->isReadyQueueAvailable()) && (!(cpuCores.empty()))) {

                for (auto& i : cpuCores) {
                    //forchecking

                    if (i->isCoreFree()) {

                        i->attachProcesstoCPUCore();

                    }

                }
            }
        }
    });

    schedulerThread.detach();

};