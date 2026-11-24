
#include <iostream>
#include <iomanip>
#include "memory/MemoryManager.h"

int main()
{
    MemoryManager manager(1000);

    manager.allocateMemory(1, 200, AllocationStrategy::FirstFit);
    manager.allocateMemory(2, 300, AllocationStrategy::FirstFit);
    manager.allocateMemory(3, 200, AllocationStrategy::FirstFit);

    manager.deallocateMemory(1);
    manager.deallocateMemory(3);

    std::cout << "MEMORY FRAGMENTATION ANALYSIS\n";
    std::cout << "-----------------------------\n";

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

    std::cout << "\nTrying to allocate 400 KB...\n";

    bool result = manager.allocateMemory(
        4,
        400,
        AllocationStrategy::FirstFit
    );

    std::cout << (result ? "Allocation successful.\n"
                         : "Allocation failed.\n");

    return 0;
}
