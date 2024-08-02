#pragma once
#include <string>

class MemoryAllocator {
public:
	virtual void* allocate(int size) = 0;
	virtual void* deallocate(void* ptr) = 0;
	virtual std::string visualizeMemory() = 0;

private:
};
