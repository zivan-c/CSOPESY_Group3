#include "MemoryAllocator.h"
#include <unordered_map>
#include <vector>
#include "Process.h"
#include <atomic>


class FirstFit : public MemoryAllocator {

public:

static FirstFit* getInstance();
static void initialize(int memoryAmount);
bool allocate(std::shared_ptr<Process> process, int processMemoryAmount); 
bool deallocate(std::shared_ptr<Process> process, int processMemoryAmount);
int totalExternalFragmentation();
std::string getDateAndTime();
void printMemoryProgress();
bool actionMemory(int action, std::shared_ptr<Process> process, int processMemoryAmount); 

int allow;
std::atomic<bool> isPrinting;


int processesinMemory;
int memorySize;
int allocatedMemory;


private: 

std::mutex queueMutex;
std::vector<std::pair<std::shared_ptr<Process>, bool> > memory;
static FirstFit* singletonInstance;
FirstFit(int size);


};
