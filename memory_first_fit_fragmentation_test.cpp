
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
    manager.allocateMemory(2, 200, AllocationStrategy::FirstFit);
    manager.allocateMemory(3, 200, AllocationStrategy::FirstFit);
    manager.allocateMemory(4, 400, AllocationStrategy::FirstFit);

    manager.deallocateMemory(1);
    manager.deallocateMemory(3);

    std::cout << "BEFORE FIRST FIT ALLOCATION\n";
    displayMemory(manager);

    std::cout << "\nAllocating P5 (150 KB) using First Fit...\n";

    bool result = manager.allocateMemory(
        5,
        150,
        AllocationStrategy::FirstFit
    );

    std::cout << (result ? "Allocation successful.\n"
                         : "Allocation failed.\n");

    std::cout << "\nAFTER FIRST FIT ALLOCATION\n";
    displayMemory(manager);

    return 0;
}
