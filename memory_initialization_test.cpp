
#include <iostream>
#include "memory/MemoryManager.h"

int main()
{
    try
    {
        MemoryManager manager(1000);

        const auto& blocks = manager.getMemoryBlocks();

        std::cout << "Total memory: "
                  << manager.getTotalMemory() << " KB\n";

        std::cout << "Number of memory blocks: "
                  << blocks.size() << "\n";

        for (const auto& block : blocks)
        {
            std::cout << "Start Address: "
                      << block.startAddress << "\n";

            std::cout << "Block Size: "
                      << block.size << " KB\n";

            std::cout << "Status: "
                      << (block.isFree ? "Free" : "Allocated")
                      << "\n";

            std::cout << "Process ID: "
                      << block.processId << "\n";
        }
    }
    catch (const std::exception& error)
    {
        std::cout << "Error: " << error.what() << "\n";
    }

    return 0;
}
