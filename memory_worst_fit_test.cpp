
#include <iostream>
#include "memory/MemoryManager.h"

int main()
{
    MemoryManager manager(1000);

    std::cout << "Testing all allocation strategies\n\n";

    std::cout << "First Fit: "
              << (manager.allocateMemory(
                      1, 200, AllocationStrategy::FirstFit)
                      ? "Success" : "Failed")
              << '\n';

    std::cout << "Best Fit: "
              << (manager.allocateMemory(
                      2, 300, AllocationStrategy::BestFit)
                      ? "Success" : "Failed")
              << '\n';

    std::cout << "Worst Fit: "
              << (manager.allocateMemory(
                      3, 100, AllocationStrategy::WorstFit)
                      ? "Success" : "Failed")
              << '\n';

    std::cout << "\nMemory Map\n";

    for (const auto& block : manager.getMemoryBlocks())
    {
        std::cout << "Start: " << block.startAddress
                  << " | Size: " << block.size << " KB"
                  << " | Status: "
                  << (block.isFree ? "Free" : "Allocated")
                  << " | PID: " << block.processId << '\n';
    }

    std::cout << "\nUsed Memory: "
              << manager.getUsedMemory() << " KB\n";

    std::cout << "Free Memory: "
              << manager.getFreeMemory() << " KB\n";

    return 0;
}
