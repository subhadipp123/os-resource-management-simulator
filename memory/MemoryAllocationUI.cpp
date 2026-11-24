
#include "MemoryAllocationUI.h"
#include "MemoryManager.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <limits>

namespace
{
    int readInteger(const std::string& prompt)
    {
        while (true)
        {
            std::cout << prompt;

            std::string input;
            std::getline(std::cin, input);

            std::stringstream stream(input);
            long long value;
            char extra;

            if ((stream >> value) &&
                !(stream >> extra) &&
                value >= std::numeric_limits<int>::min() &&
                value <= std::numeric_limits<int>::max())
            {
                return static_cast<int>(value);
            }

            std::cout << "Invalid input. Enter a valid integer.\n";
        }
    }

    AllocationStrategy selectStrategy()
    {
        std::cout << "\nALLOCATION STRATEGIES\n";
        std::cout << "1. First Fit\n";
        std::cout << "2. Best Fit\n";
        std::cout << "3. Worst Fit\n";

        while (true)
        {
            int choice = readInteger("Select strategy: ");

            switch (choice)
            {
                case 1:
                    return AllocationStrategy::FirstFit;

                case 2:
                    return AllocationStrategy::BestFit;

                case 3:
                    return AllocationStrategy::WorstFit;

                default:
                    std::cout << "Invalid strategy. Try again.\n";
            }
        }
    }

    void displayMemoryMap(const MemoryManager& manager)
    {
        std::cout << "\n========== MEMORY MAP ==========\n";

        std::cout << std::left
                  << std::setw(15) << "Start Address"
                  << std::setw(12) << "Size (KB)"
                  << std::setw(15) << "Status"
                  << std::setw(12) << "PID" << '\n';

        std::cout << std::string(54, '-') << '\n';

        for (const auto& block : manager.getMemoryBlocks())
        {
            std::cout << std::left
                      << std::setw(15) << block.startAddress
                      << std::setw(12) << block.size
                      << std::setw(15)
                      << (block.isFree ? "Free" : "Allocated")
                      << std::setw(12) << block.processId
                      << '\n';
        }

        std::cout << std::string(54, '-') << '\n';
    }

    void displayMemoryStatistics(const MemoryManager& manager)
    {
        std::cout << "\n======= MEMORY STATISTICS =======\n";

        std::cout << "Total Memory: "
                  << manager.getTotalMemory() << " KB\n";

        std::cout << "Used Memory: "
                  << manager.getUsedMemory() << " KB\n";

        std::cout << "Free Memory: "
                  << manager.getFreeMemory() << " KB\n";

        std::cout << "Memory Utilization: "
                  << std::fixed << std::setprecision(2)
                  << manager.getMemoryUtilization() << "%\n";

        std::cout << "Largest Free Block: "
                  << manager.getLargestFreeBlock() << " KB\n";

        std::cout << "External Fragmentation: "
                  << manager.getExternalFragmentation() << " KB\n";

        std::cout << "External Fragmentation Percentage: "
                  << manager.getExternalFragmentationPercentage()
                  << "%\n";

        std::cout << "=================================\n";
    }
}

void runMemoryManagement()
{
    std::cout << "\n========================================\n";
    std::cout << "       MEMORY MANAGEMENT MODULE\n";
    std::cout << "========================================\n";

    int totalMemory;

    do
    {
        totalMemory = readInteger("Enter total memory size (KB): ");

        if (totalMemory <= 0)
        {
            std::cout << "Memory size must be greater than zero.\n";
        }

    } while (totalMemory <= 0);

    MemoryManager manager(totalMemory);

    while (true)
    {
        std::cout << "\n========== MEMORY MANAGEMENT ==========\n";
        std::cout << "1. Allocate Memory\n";
        std::cout << "2. Deallocate Memory\n";
        std::cout << "3. Display Memory Map\n";
        std::cout << "4. Display Memory Statistics\n";
        std::cout << "5. Return to Main Menu\n";
        std::cout << "========================================\n";

        int choice = readInteger("Enter your choice: ");

        switch (choice)
        {
            case 1:
            {
                int processId = readInteger("Enter Process ID: ");
                int requestedSize =
                    readInteger("Enter memory size (KB): ");

                AllocationStrategy strategy = selectStrategy();

                if (manager.allocateMemory(
                        processId, requestedSize, strategy))
                {
                    std::cout << "\nMemory allocated successfully.\n";
                }
                else
                {
                    std::cout << "\nMemory allocation failed.\n";
                    std::cout << "Check the PID, requested size "
                                 "and available memory.\n";
                }

                break;
            }

            case 2:
            {
                int processId = readInteger("Enter Process ID: ");

                if (manager.deallocateMemory(processId))
                {
                    std::cout << "\nProcess memory released successfully.\n";
                }
                else
                {
                    std::cout << "\nDeallocation failed. "
                                 "Process ID not found or invalid.\n";
                }

                break;
            }

            case 3:
                displayMemoryMap(manager);
                break;

            case 4:
                displayMemoryStatistics(manager);
                break;

            case 5:
                std::cout << "\nReturning to Main Menu...\n";
                return;

            default:
                std::cout << "\nInvalid choice. Try again.\n";
        }
    }
}
