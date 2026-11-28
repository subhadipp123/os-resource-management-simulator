
#include "DeadlockUI.h"
#include "graph/ResourceAllocationGraph.h"
#include "graph/GraphRenderer.h"
#include "detection/DeadlockDetector.h"

#include <iostream>
#include <limits>
#include <string>

namespace
{
    bool readInteger(const std::string& prompt, int& value,
                     bool positiveOnly = false)
    {
        std::cout << prompt;

        if (!(std::cin >> value))
        {
            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n'
            );

            std::cout << "Invalid numeric input.\n";
            return false;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n'
        );

        if (value < 0 || (positiveOnly && value == 0))
        {
            std::cout << (positiveOnly
                ? "Value must be positive.\n"
                : "Value cannot be negative.\n");

            return false;
        }

        return true;
    }

    void printResult(const DetectionResult& result)
    {
        std::cout << "\n===== DEADLOCK DETECTION RESULT =====\n";

        switch (result.status)
        {
            case DetectionStatus::NoDeadlock:
                std::cout << "NO DEADLOCK DETECTED.\n";
                break;

            case DetectionStatus::DeadlockDetected:
                std::cout << "DEADLOCK DETECTED!\n";
                std::cout << "Deadlocked processes: ";

                for (int id : result.deadlockedProcessIds)
                {
                    std::cout << "P" << id << " ";
                }

                std::cout << '\n';
                break;

            case DetectionStatus::UnsupportedMultipleInstances:
                std::cout
                    << "Multiple-instance resources are unsupported "
                    << "by this detection mode.\n";
                break;

            case DetectionStatus::InvalidGraph:
                std::cout << "INVALID RESOURCE ALLOCATION GRAPH.\n";
                break;
        }
    }
}

void runDeadlockManagement()
{
    std::cout << "\n========================================\n"
              << "          DEADLOCK MANAGEMENT\n"
              << "========================================\n";

    int processCount = 0;
    int resourceCount = 0;

    if (!readInteger("Enter number of processes: ",
                     processCount, true))
        return;

    if (!readInteger("Enter number of resource types: ",
                     resourceCount, true))
        return;

    ResourceAllocationGraph graph;

    // Create processes
    for (int i = 0; i < processCount; ++i)
    {
        if (!graph.addProcess(i))
        {
            std::cout << "Could not create process.\n";
            return;
        }
    }

    // Create resources
    for (int j = 0; j < resourceCount; ++j)
    {
        int total = 0;

        if (!readInteger(
                "Total instances of R" + std::to_string(j) + ": ",
                total, true))
            return;

        if (!graph.addResource(j, total))
        {
            std::cout << "Could not create resource.\n";
            return;
        }
    }

    // Enter allocations
    std::cout << "\nEnter allocation quantities "
              << "(0 means no allocation).\n";

    for (int i = 0; i < processCount; ++i)
    {
        for (int j = 0; j < resourceCount; ++j)
        {
            int units = 0;

            if (!readInteger(
                    "R" + std::to_string(j) +
                    " allocated to P" + std::to_string(i) + ": ",
                    units))
                return;

            if (units > 0 && !graph.addAllocation(i, j, units))
            {
                std::cout
                    << "Allocation rejected: insufficient available "
                    << "instances or invalid data.\n";
                return;
            }
        }
    }

    // Enter outstanding requests
    std::cout << "\nEnter outstanding request quantities "
              << "(0 means no request).\n";

    for (int i = 0; i < processCount; ++i)
    {
        for (int j = 0; j < resourceCount; ++j)
        {
            int units = 0;

            if (!readInteger(
                    "R" + std::to_string(j) +
                    " requested by P" + std::to_string(i) + ": ",
                    units))
                return;

            if (units > 0 && !graph.addRequest(i, j, units))
            {
                std::cout << "Request rejected: invalid data.\n";
                return;
            }
        }
    }

    // Display graph and detect deadlock
    graph.displayGraph();

    DetectionResult result =
        DeadlockDetector::detectMultipleInstanceDeadlock(graph);

    printResult(result);

    std::cout
        << "\nOpening graphical Resource Allocation Graph. "
        << "Close its window to return to the main menu.\n";

    GraphRenderer renderer(graph);
    renderer.run();
}
