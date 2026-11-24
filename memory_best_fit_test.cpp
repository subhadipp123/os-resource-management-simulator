
#include <iostream>
#include "memory/MemoryManager.h"

void displayMemory(const MemoryManager& manager)
{
    for (const auto& block : manager.getMemoryBlocks())
    {
        std::cout << "Start: " << block.startAddress
                  << " | Size: " << block.size << " KB"
                  << " | Status: "
                  << (block.isFree ? "Free" : "Allocated")
                  << " | PID: " << block.processId << '\n';
    }
}

int main()
{
    MemoryManager manager(1000);

    manager.allocateMemory(1, 200, AllocationStrategy::FirstFit);
    manager.allocateMemory(2, 300, AllocationStrategy::FirstFit);
    manager.allocateMemory(3, 100, AllocationStrategy::FirstFit);

    std::cout << "Initial Memory Map:\n";
    displayMemory(manager);

    std::cout << "\nDeallocation is not implemented yet.\n";
    std::cout << "Best Fit will be tested after we implement "
                 "deallocation and create multiple free blocks.\n";

    return 0;
}
