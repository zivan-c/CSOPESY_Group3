#include "PreemptiveSJF.h"
#include "CPUScheduler.h"



void PreemptiveSJF::runScheduler() {

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
                    else {

                        i->getProcessinCPUCore()->setProcessState(Process::WAITING); //set to waiting to prevent instruction execution
                        if ((i->getProcessinCPUCore()->getRemainingInstructions()) <
                            CPUScheduler::getInstance()->returnLowestRemainingInstructions()) {

                            i->attachProcesstoCPUCore();

                        }
                    }
                }
            }
        }
        });


    schedulerThread.detach();

};