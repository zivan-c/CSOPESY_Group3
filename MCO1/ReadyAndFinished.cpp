#include "ReadyAndFinished.h"

ReadyAndFinished* ReadyAndFinished::singletonInstance = nullptr;
ReadyAndFinished* ReadyAndFinished::getInstance() { return singletonInstance; };

void ReadyAndFinished::initialize() {

    singletonInstance = new ReadyAndFinished();

}

//Returns a process from the ready queue and removes it from the list
std::shared_ptr<Process> ReadyAndFinished::returnProcess() {

    std::lock_guard<std::mutex> lock(mtx);

    if (!(readyQueue.empty())) {
        auto front = readyQueue.front();
        readyQueue.erase(readyQueue.begin());
        return front;
    }
    else {
        return nullptr;
    }

};

//Pushes the passed process pointer to the back of the ready queue
void ReadyAndFinished::pushProcessToReady(std::shared_ptr<Process> process) {

    std::lock_guard<std::mutex> lock(mtx);
    readyQueue.push_back(process);

};

//Pushes a finished process to the finished list
void ReadyAndFinished::pushProcessToFinished(std::shared_ptr<Process> process) {

    std::lock_guard<std::mutex> lock(mtx);
    finishedList.push_back(process);

};

std::shared_ptr<Process> ReadyAndFinished::returnProcessInReady(std::string processName) {

    std::lock_guard<std::mutex> lock(mtx);
    std::shared_ptr<Process> placeholder = nullptr;

    for (auto i : readyQueue) {
        if (i->processName == processName) {
            placeholder = i;
        }
    }

    return placeholder;

};