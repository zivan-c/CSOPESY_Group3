#include "MemoryAllocator.h"


class FirstFit : public MemoryAllocator {

public:
FirstFit(int size);


private: 
int memorySize;



};
