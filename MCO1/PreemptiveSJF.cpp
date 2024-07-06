#include "PreemptiveSJF.h"
#include "CPUScheduler.h"



void PreemptiveSJF::runScheduler() {

    std::thread schedulerThread;

    schedulerThread = std::thread([this]() {

        std::vector<std::shared_ptr<CPUCore>>& cpuCores = CPUScheduler::getInstance()->cpuCores;

        while (isRunning) {

            if ((CPUScheduler::getInstance()->isReadyQueueAvailable()) && (!(CPUScheduler::getInstance()->cpuCores.empty()))) {

                for (auto& i : cpuCores) {

                    if (i->isCoreFree()) {

                        i->getProcessFromReadyQueue();

                    }
                    else {

                        i->getProcessinCPUCore()->setProcessState(Process::ProcessState::WAITING); //set to waiting to prevent instruction execution
                        if ((i->getProcessinCPUCore()->getRemainingInstructions()) >
                            CPUScheduler::getInstance()->returnLowestRemainingInstructions()) {

                            i->returnProcesstoReadyQueue();
                            i->getProcessFromReadyQueue();
                            CPUScheduler::getInstance()->sortReadyQueue();


                        }
                        else {
                            i->getProcessinCPUCore()->setProcessState(Process::ProcessState::PROCESSING);
                        }

                    }
                }
            }
        }
        });


    schedulerThread.detach();

};