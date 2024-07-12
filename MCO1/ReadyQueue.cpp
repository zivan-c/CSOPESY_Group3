#include "ReadyQueue.h"
#include <vector>
#include <memory>
#include <algorithm>

ReadyQueue *ReadyQueue::singletonInstance = nullptr;
ReadyQueue* ReadyQueue::getInstance() { return singletonInstance; };

void ReadyQueue::initialize() {

  singletonInstance = new ReadyQueue();

}



std::shared_ptr<Process> ReadyQueue::removeProcessFromReadyQueue(){
  std::lock_guard<std::mutex> lock(queueMutex); 
  if (!readyQueue.empty()) {
    auto front = readyQueue.front();
    readyQueue.erase(readyQueue.begin());
    return front;
  } else {
      return nullptr; // or throw an exception, handle as appropriate
  }


};


int ReadyQueue::returnLowestRemainingInstructions(){
  std::lock_guard<std::mutex> lock(queueMutex); 
  auto front = readyQueue.front();
  int lowestInstructions = front->getRemainingInstructions();
  return lowestInstructions;


};
bool ReadyQueue::isReadyQueueAvailable(){


  std::lock_guard<std::mutex> lock(queueMutex); 

  if(!(readyQueue.empty())){
    return true;
  }else{
    return false;
  }

};

void ReadyQueue::sortReadyQueue() {

  std::lock_guard<std::mutex> lock(queueMutex); 

    std::sort(readyQueue.begin(), readyQueue.end(), [](const std::shared_ptr<Process>& a, const std::shared_ptr<Process>& b) {
        return a->getRemainingInstructions() < b->getRemainingInstructions();
    });
}


void ReadyQueue::pushToReadyQueue(std::shared_ptr<Process> process){
  std::lock_guard<std::mutex> lock(queueMutex); 

    readyQueue.push_back(process);

};

std::shared_ptr<Process> ReadyQueue::getProcessPointer(std::string process){
  //std::lock_guard<std::mutex> lock(queueMutex); 
  
  if(!(readyQueue.empty())){
    for(auto& i : readyQueue){
      if(i->getProcessName() == process){
        return i;
      }
    }
  }

  return nullptr;


};



