
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

    std::cout << "INITIAL MEMORY MAP\n";
    displayMemory(manager);

    std::cout << "\nDeallocating P2...\n";

    std::cout << (manager.deallocateMemory(2)
        ? "P2 deallocated successfully.\n"
        : "P2 deallocation failed.\n");

    std::cout << "\nMEMORY MAP AFTER P2 DEALLOCATION\n";
    displayMemory(manager);

    std::cout << "\nDeallocating P1...\n";

    std::cout << (manager.deallocateMemory(1)
        ? "P1 deallocated successfully.\n"
        : "P1 deallocation failed.\n");

    std::cout << "\nMEMORY MAP AFTER P1 DEALLOCATION\n";
    displayMemory(manager);

    std::cout << "\nDeallocating P3...\n";

    std::cout << (manager.deallocateMemory(3)
        ? "P3 deallocated successfully.\n"
        : "P3 deallocation failed.\n");

    std::cout << "\nFINAL MEMORY MAP\n";
    displayMemory(manager);

    std::cout << "\nFinal Free Memory: "
              << manager.getFreeMemory() << " KB\n";

    std::cout << "Largest Free Block: "
              << manager.getLargestFreeBlock() << " KB\n";

    return 0;
}
