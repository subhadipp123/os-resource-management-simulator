
#include <iostream>
#include "memory/MemoryManager.h"

int main()
{
    MemoryManager manager(1000);

    std::cout << "Initial Memory: "
              << manager.getFreeMemory() << " KB\n";

    std::cout << "\nAllocating P1 (200 KB)...\n";

    if (manager.allocateMemory(
            1, 200, AllocationStrategy::FirstFit))
    {
        std::cout << "P1 allocated successfully.\n";
    }
    else
    {
        std::cout << "P1 allocation failed.\n";
    }

    std::cout << "\nAllocating P2 (300 KB)...\n";

    if (manager.allocateMemory(
            2, 300, AllocationStrategy::FirstFit))
    {
        std::cout << "P2 allocated successfully.\n";
    }
    else
    {
        std::cout << "P2 allocation failed.\n";
    }

    std::cout << "\nMEMORY MAP\n";

    for (const auto& block : manager.getMemoryBlocks())
    {
        std::cout << "Start: " << block.startAddress
                  << " | Size: " << block.size << " KB"
                  << " | Status: "
                  << (block.isFree ? "Free" : "Allocated")
                  << " | PID: " << block.processId << '\n';
    }

    std::cout << "\nMemory Statistics\n";

    std::cout << "Total: "
              << manager.getTotalMemory() << " KB\n";

    std::cout << "Used: "
              << manager.getUsedMemory() << " KB\n";

    std::cout << "Free: "
              << manager.getFreeMemory() << " KB\n";

    std::cout << "Utilization: "
              << manager.getMemoryUtilization() << "%\n";

    std::cout << "Largest Free Block: "
              << manager.getLargestFreeBlock() << " KB\n";

    return 0;
}
