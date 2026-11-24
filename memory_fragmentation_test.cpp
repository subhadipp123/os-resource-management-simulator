
#include <iostream>
#include <iomanip>

#include "memory/MemoryManager.h"

int main()
{
    MemoryManager manager(1000);

    // Create allocated and free regions.
    manager.allocateMemory(1, 200, AllocationStrategy::FirstFit);
    manager.allocateMemory(2, 100, AllocationStrategy::FirstFit);
    manager.allocateMemory(3, 300, AllocationStrategy::FirstFit);
    manager.allocateMemory(4, 100, AllocationStrategy::FirstFit);

    // Release P1 and P3 to create separate free blocks.
    manager.deallocateMemory(1);
    manager.deallocateMemory(3);

    std::cout << "MEMORY MAP\n";

    for (const auto& block : manager.getMemoryBlocks())
    {
        std::cout << "Start: " << block.startAddress
                  << " | Size: " << block.size << " KB"
                  << " | Status: "
                  << (block.isFree ? "Free" : "Allocated")
                  << " | PID: " << block.processId << '\n';
    }

    std::cout << "\nMEMORY STATISTICS\n";

    std::cout << "Total Memory: "
              << manager.getTotalMemory() << " KB\n";

    std::cout << "Used Memory: "
              << manager.getUsedMemory() << " KB\n";

    std::cout << "Free Memory: "
              << manager.getFreeMemory() << " KB\n";

    std::cout << "Largest Free Block: "
              << manager.getLargestFreeBlock() << " KB\n";

    std::cout << "External Fragmentation: "
              << manager.getExternalFragmentation() << " KB\n";

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "External Fragmentation Percentage: "
              << manager.getExternalFragmentationPercentage()
              << "%\n";

    std::cout << "Memory Utilization: "
              << manager.getMemoryUtilization() << "%\n";

    return 0;
}
