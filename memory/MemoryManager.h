
#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <vector>
#include "MemoryBlock.h"

enum class AllocationStrategy
{
    FirstFit,
    BestFit,
    WorstFit
};

class MemoryManager
{
private:
    int totalMemory;
    std::vector<MemoryBlock> memoryBlocks;

    void mergeAdjacentFreeBlocks();

public:
    explicit MemoryManager(int totalMemory);

    bool allocateMemory(
        int processId,
        int requestedSize,
        AllocationStrategy strategy
    );

    bool deallocateMemory(int processId);

    const std::vector<MemoryBlock>& getMemoryBlocks() const;

    int getTotalMemory() const;
    int getUsedMemory() const;
    int getFreeMemory() const;

    double getMemoryUtilization() const;

    int getLargestFreeBlock() const;

    int getExternalFragmentation() const;

    double getExternalFragmentationPercentage() const;
};

#endif
