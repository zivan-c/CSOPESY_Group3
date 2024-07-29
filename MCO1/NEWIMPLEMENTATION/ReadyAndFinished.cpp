#include "ReadyAndFinished.h"


/*
  void pushProcessToReady(std::shared_ptr<Process> process);
  void pushProcessToFinished(std::shared_ptr<Process> process);
  std::shared_ptr<Process> returnProcess();
  std::vector<std::shared_ptr<Process>> finishedList;

  static void initialize();
  static ReadyAndFinished* getInstance(); 
*/


ReadyAndFinished *ReadyAndFinished::singletonInstance = nullptr;
ReadyAndFinished* ReadyAndFinished::getInstance() { return singletonInstance; };

void ReadyAndFinished::initialize() {

  singletonInstance = new ReadyAndFinished();

}

std::shared_ptr<Process> ReadyAndFinished::returnProcess(){
  
  std::lock_guard<std::mutex> lock(mtx);

  if (!(readyQueue.empty())) {
    auto front = readyQueue.front();
    readyQueue.erase(readyQueue.begin());
    return front;
  } else {
      return nullptr; 
  }

};

void ReadyAndFinished::pushProcessToReady(std::shared_ptr<Process> process){

  std::lock_guard<std::mutex> lock(mtx);
  readyQueue.push_back(process);

};

void ReadyAndFinished::pushProcessToFinished(std::shared_ptr<Process> process){

  std::lock_guard<std::mutex> lock(mtx);
  finishedList.push_back(process);

};
