#include "RRScheduler.h"
#include "CPUScheduler.h"
#include <thread>
#include "FirstFit.h"

void RRScheduler::runScheduler(){

  std::thread schedulerThread;
  isRunning = 1;

  schedulerThread = std::thread([this](){

    //Implementation is within the CPUCore class
    //
    CPUScheduler::getInstance()->quantumCycleAmount++; 
    FirstFit::getInstance()->printMemoryProgress();
    std::chrono::duration<float, std::milli> delayDuration(0.003 * 1000);
    std::this_thread::sleep_for(delayDuration);

  });
  
  
  schedulerThread.detach();

};
