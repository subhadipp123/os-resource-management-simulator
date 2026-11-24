
#include <iostream>
#include "memory/MemoryManager.h"

void displayMemory(const MemoryManager& manager)
{
    std::cout << "\nMEMORY MAP\n";

    for (const auto& block : manager.getMemoryBlocks())
    {
        std::cout << "Start: " << block.startAddress
                  << " | Size: " << block.size << " KB"
                  << " | Status: "
                  << (block.isFree ? "Free" : "Allocated")
                  << " | PID: " << block.processId << '\n';
    }

    std::cout << "Used Memory: "
              << manager.getUsedMemory() << " KB\n";

    std::cout << "Free Memory: "
              << manager.getFreeMemory() << " KB\n";
}

int main()
{
    MemoryManager manager(1000);

    manager.allocateMemory(1, 200, AllocationStrategy::FirstFit);
    manager.allocateMemory(2, 300, AllocationStrategy::FirstFit);
    manager.allocateMemory(3, 200, AllocationStrategy::FirstFit);

    std::cout << "BEFORE DEALLOCATION";
    displayMemory(manager);

    std::cout << "\nDeallocating P2...\n";

    if (manager.deallocateMemory(2))
        std::cout << "P2 deallocated successfully.\n";
    else
        std::cout << "Deallocation failed.\n";

    displayMemory(manager);

    std::cout << "\nDeallocating P1...\n";

    if (manager.deallocateMemory(1))
        std::cout << "P1 deallocated successfully.\n";
    else
        std::cout << "Deallocation failed.\n";

    displayMemory(manager);

    std::cout << "\nTrying to deallocate P1 again...\n";

    std::cout << (manager.deallocateMemory(1)
        ? "Unexpected success.\n"
        : "Correctly rejected.\n");

    return 0;
}
