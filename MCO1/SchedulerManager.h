#pragma once

#include "CPUCore.h"

#include <vector>
#include <string>

class SchedulerManager {

public:

    static SchedulerManager* getInstance();

    int isRunning;
    static int isCreatingProcesses;

    int CPUCoreCount;
    int quantumCycleCount;
    float creationDelay;
    float executionDelay;

    int instructionsLowerBound;
    int instructionsHigherBound;

    int memoryLowerBound;
    int memoryHigherBound;

    int pageCount;

    static void initialize(int cpuCount, int qCount, float cDelay, float eDelay,
        int iLowerBound, int iHigherBound, int mLowerBound,
        int mHigherBound, int pCount);


    void setupCores();
    void createProcesses();
    std::string generateProcessName();

    void screenLS();
    void reportUtil();

    std::vector<std::shared_ptr<CPUCore> > coreList;

private:

    static SchedulerManager* singletonInstance;


};