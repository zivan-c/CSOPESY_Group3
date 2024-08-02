#include <vector>
#include <unordered_map>
#include "FlatMemoryAllocator.h"

FlatMemoryAllocator::FlatMemoryAllocator(int maximumSize) {
    memory.reserve(maximumSize);
    maxSize = maximumSize;
    initMemory();
}

FlatMemoryAllocator::~FlatMemoryAllocator() {
    memory.clear();
}

int FlatMemoryAllocator::getAllocatedProcessCount() {
    return allocatedSize;
}

void FlatMemoryAllocator::initMemory() {
    std::fill(memory.begin(), memory.end(), '.');
    allocationMap.clear();
    for (int i = 0; i < maxSize; i++) {
        allocationMap[i] = false;
    }
}

bool FlatMemoryAllocator::canAllocateAt(int i, int size) const {
    return (i + size <= maxSize);
}

void FlatMemoryAllocator::allocateAt(int i, int s) {
    for (int j = i; j < i + s; j++) {
        allocationMap[j] = true;
    }
    allocatedSize += s;
}

void FlatMemoryAllocator::deallocateAt(int i) {
    allocationMap[i] = false;
}

void* FlatMemoryAllocator::allocate(int size) {
    for (int i = 0; i < maxSize - size + 1; i++) {
        if (!allocationMap[i] && canAllocateAt(i, size)) {
            allocateAt(i, size);
            return &memory[i];
        }
    }
    return nullptr; // Or any appropriate error handling
}

void* FlatMemoryAllocator::deallocate(void* ptr) {
    size_t i = static_cast<char*>(ptr) - &memory[0];
    if (allocationMap[i]) {
        deallocateAt(i);
    }
    return nullptr;
}

std::string FlatMemoryAllocator::visualizeMemory() {
    return std::string(memory.begin(), memory.end());
}

int FlatMemoryAllocator::getExternalFragmentation() {
    int freeBlocks = 0;
    int largestFreeBlock = 0;
    int currentFreeBlockSize = 0;

    for (int i = 0; i < maxSize; i++) {
        if (!allocationMap[i]) {
            currentFreeBlockSize++;
        }
        else {
            if (currentFreeBlockSize > 0) {
                freeBlocks++;
                largestFreeBlock = std::max(largestFreeBlock, currentFreeBlockSize);
                currentFreeBlockSize = 0;
            }
        }
    }

    // Handle the last free block, if any
    if (currentFreeBlockSize > 0) {
        freeBlocks++;
        largestFreeBlock = std::max(largestFreeBlock, currentFreeBlockSize);
    }

    return (largestFreeBlock == maxSize) ? 0 : (maxSize - largestFreeBlock - allocatedSize) * 100 / maxSize;
}