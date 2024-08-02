#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "MemoryAllocator.h"

class FlatMemoryAllocator : public MemoryAllocator {
public:
	FlatMemoryAllocator(int maxSize);
	~FlatMemoryAllocator();
	void* allocate(int size) override;
	void* deallocate(void* ptr) override;
	std::string visualizeMemory() override;
	int getAllocatedProcessCount();
	int getExternalFragmentation(); // Added this function definition

private:
	int allocatedSize;
	int maxSize;
	int minSize;
	std::vector<char> memory;
	std::unordered_map<int, bool> allocationMap;

	void initMemory();
	bool canAllocateAt(int ind, int size) const;
	void allocateAt(int ind, int size);
	void deallocateAt(int ind);

};