
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

void testStrategy(
    const char* strategyName,
    AllocationStrategy strategy)
{
    MemoryManager manager(1000);

    manager.allocateMemory(1, 300, AllocationStrategy::FirstFit);
    manager.allocateMemory(2, 200, AllocationStrategy::FirstFit);
    manager.allocateMemory(3, 500, AllocationStrategy::FirstFit);

    manager.deallocateMemory(1);
    manager.deallocateMemory(3);

    std::cout << "\n" << strategyName << "\n";
    std::cout << "Before allocation:\n";
    displayMemory(manager);

    bool result = manager.allocateMemory(4, 250, strategy);

    std::cout << "\nAllocation result: "
              << (result ? "Success" : "Failed") << '\n';

    std::cout << "After allocation:\n";
    displayMemory(manager);
}

int main()
{
    testStrategy("FIRST FIT", AllocationStrategy::FirstFit);
    testStrategy("BEST FIT", AllocationStrategy::BestFit);
    testStrategy("WORST FIT", AllocationStrategy::WorstFit);

    return 0;
}
