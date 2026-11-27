
#include "deadlock/detection/DeadlockDetector.h"

#include <iostream>

void printResult(const DetectionResult& result)
{
    if (result.status == DetectionStatus::DeadlockDetected)
    {
        std::cout << "DEADLOCK DETECTED!\n";
        std::cout << "Deadlocked processes: ";

        for (int id : result.deadlockedProcessIds)
        {
            std::cout << "P" << id << " ";
        }

        std::cout << "\n";
    }
    else if (result.status == DetectionStatus::NoDeadlock)
    {
        std::cout << "NO DEADLOCK DETECTED.\n";
    }
    else
    {
        std::cout << "UNSUPPORTED OR INVALID GRAPH.\n";
    }
}

int main()
{
    std::cout << "===== MULTIPLE-INSTANCE DEADLOCK DETECTION =====\n";

    // TEST A: Deadlock

    ResourceAllocationGraph graph1;

    graph1.addProcess(0);
    graph1.addProcess(1);

    graph1.addResource(0, 3);
    graph1.addResource(1, 2);

    graph1.addAllocation(0, 0, 2);
    graph1.addAllocation(1, 1, 1);

    graph1.addRequest(0, 1, 2);
    graph1.addRequest(1, 0, 2);

    std::cout << "\nTEST A: Multiple-instance deadlock\n";

    graph1.displayGraph();

    DetectionResult result1 =
        DeadlockDetector::detectMultipleInstanceDeadlock(graph1);

    printResult(result1);

    // TEST B: No deadlock

    ResourceAllocationGraph graph2;

    graph2.addProcess(0);
    graph2.addProcess(1);

    graph2.addResource(0, 3);
    graph2.addResource(1, 2);

    graph2.addAllocation(0, 0, 2);
    graph2.addAllocation(1, 1, 1);

    graph2.addRequest(0, 1, 1);
    graph2.addRequest(1, 0, 1);

    std::cout << "\nTEST B: No deadlock\n";

    graph2.displayGraph();

    DetectionResult result2 =
        DeadlockDetector::detectMultipleInstanceDeadlock(graph2);

    printResult(result2);

    return 0;
}
