
#include "MemoryManager.h"

#include <stdexcept>

MemoryManager::MemoryManager(int totalMemory)
    : totalMemory(totalMemory)
{
    if (totalMemory <= 0)
    {
        throw std::invalid_argument(
            "Total memory must be greater than zero."
        );
    }

    memoryBlocks.push_back({
        0,
        totalMemory,
        true,
        -1
    });
}

const std::vector<MemoryBlock>& MemoryManager::getMemoryBlocks() const
{
    return memoryBlocks;
}

int MemoryManager::getTotalMemory() const
{
    return totalMemory;
}

int MemoryManager::getUsedMemory() const
{
    int usedMemory = 0;

    for (const auto& block : memoryBlocks)
    {
        if (!block.isFree)
        {
            usedMemory += block.size;
        }
    }

    return usedMemory;
}

int MemoryManager::getFreeMemory() const
{
    int freeMemory = 0;

    for (const auto& block : memoryBlocks)
    {
        if (block.isFree)
        {
            freeMemory += block.size;
        }
    }

    return freeMemory;
}

double MemoryManager::getMemoryUtilization() const
{
    return (static_cast<double>(getUsedMemory()) /
            totalMemory) * 100.0;
}

int MemoryManager::getLargestFreeBlock() const
{
    int largestBlock = 0;

    for (const auto& block : memoryBlocks)
    {
        if (block.isFree && block.size > largestBlock)
        {
            largestBlock = block.size;
        }
    }

    return largestBlock;
}

bool MemoryManager::allocateMemory(
    int processId,
    int requestedSize,
    AllocationStrategy strategy)
{
    if (processId <= 0 || requestedSize <= 0)
    {
        return false;
    }

    for (const auto& block : memoryBlocks)
    {
        if (!block.isFree && block.processId == processId)
        {
            return false;
        }
    }

    if (strategy != AllocationStrategy::FirstFit &&
        strategy != AllocationStrategy::BestFit &&
        strategy != AllocationStrategy::WorstFit)
    {
        return false;
    }

    int selectedIndex = -1;

    for (std::size_t i = 0; i < memoryBlocks.size(); i++)
    {
        const auto& block = memoryBlocks[i];

        if (!block.isFree || block.size < requestedSize)
        {
            continue;
        }

        if (strategy == AllocationStrategy::FirstFit)
        {
            selectedIndex = static_cast<int>(i);
            break;
        }

        if (strategy == AllocationStrategy::BestFit)
        {
            if (selectedIndex == -1 ||
                block.size < memoryBlocks[selectedIndex].size)
            {
                selectedIndex = static_cast<int>(i);
            }
        }

        if (strategy == AllocationStrategy::WorstFit)
        {
            if (selectedIndex == -1 ||
                block.size > memoryBlocks[selectedIndex].size)
            {
                selectedIndex = static_cast<int>(i);
            }
        }
    }

    if (selectedIndex == -1)
    {
        return false;
    }

    MemoryBlock& selectedBlock = memoryBlocks[selectedIndex];

    int remainingSize = selectedBlock.size - requestedSize;

    int remainingStartAddress =
        selectedBlock.startAddress + requestedSize;

    selectedBlock.size = requestedSize;
    selectedBlock.isFree = false;
    selectedBlock.processId = processId;

    if (remainingSize > 0)
    {
        MemoryBlock remainingBlock = {
            remainingStartAddress,
            remainingSize,
            true,
            -1
        };

        memoryBlocks.insert(
            memoryBlocks.begin() + selectedIndex + 1,
            remainingBlock
        );
    }

    return true;
}

bool MemoryManager::deallocateMemory(int processId)
{
    if (processId <= 0)
    {
        return false;
    }

    for (auto& block : memoryBlocks)
    {
        if (!block.isFree && block.processId == processId)
        {
            block.isFree = true;
            block.processId = -1;

            mergeAdjacentFreeBlocks();

            return true;
        }
    }

    return false;
}

void MemoryManager::mergeAdjacentFreeBlocks()
{
    std::size_t i = 0;

    while (i + 1 < memoryBlocks.size())
    {
        if (memoryBlocks[i].isFree &&
            memoryBlocks[i + 1].isFree)
        {
            memoryBlocks[i].size += memoryBlocks[i + 1].size;

            memoryBlocks.erase(
                memoryBlocks.begin() + i + 1
            );
        }
        else
        {
            i++;
        }
    }
}


int MemoryManager::getExternalFragmentation() const
{
    return getFreeMemory() - getLargestFreeBlock();
}

double MemoryManager::getExternalFragmentationPercentage() const
{
    int freeMemory = getFreeMemory();

    if (freeMemory == 0)
    {
        return 0.0;
    }

    return (static_cast<double>(getExternalFragmentation()) /
            freeMemory) * 100.0;
}
