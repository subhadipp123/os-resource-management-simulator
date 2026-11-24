
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
    manager.allocateMemory(2, 100, AllocationStrategy::FirstFit);
    manager.allocateMemory(3, 400, AllocationStrategy::FirstFit);
    manager.allocateMemory(4, 100, AllocationStrategy::FirstFit);
    manager.allocateMemory(5, 200, AllocationStrategy::FirstFit);

    manager.deallocateMemory(1);
    manager.deallocateMemory(3);

    std::cout << "BEFORE WORST FIT ALLOCATION\n";
    displayMemory(manager);

    std::cout << "\nAllocating P6 (150 KB) using Worst Fit...\n";

    bool result = manager.allocateMemory(
        6,
        150,
        AllocationStrategy::WorstFit
    );

    std::cout << (result ? "Allocation successful.\n"
                         : "Allocation failed.\n");

    std::cout << "\nAFTER WORST FIT ALLOCATION\n";
    displayMemory(manager);

    return 0;
}
