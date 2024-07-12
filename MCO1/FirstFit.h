#include "MemoryAllocator.h"
#include <unordered_map>
#include <vector>
#include "Process.h"


class FirstFit : public MemoryAllocator {

public:

static FirstFit* getInstance();
static void initialize(int memoryAmount);
bool allocate(std::shared_ptr<Process>, int processMemoryAmount); 
void deallocate(std::shared_ptr<Process>, int processMemoryAmount);
int totalExternalFragmentation();
std::string getDateAndTime();
void printMemoryProgress();


int processesinMemory;
int memorySize;
int allocatedMemory;


private: 

std::mutex queueMutex;
std::vector<std::pair<std::shared_ptr<Process>, bool> > memory;
static FirstFit* singletonInstance;
FirstFit(int size);


};
