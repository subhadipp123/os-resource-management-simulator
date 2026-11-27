
#include "deadlock/detection/DeadlockDetector.h"

#include <iostream>

void printResult(const DetectionResult& result)
{
    if (result.status == DetectionStatus::DeadlockDetected)
    {
        std::cout << "\nDEADLOCK DETECTED!\n";

        std::cout << "Deadlocked processes: ";

        for (int processId : result.deadlockedProcessIds)
        {
            std::cout << "P" << processId << " ";
        }

        std::cout << "\n";
    }
    else if (result.status == DetectionStatus::NoDeadlock)
    {
        std::cout << "\nNO DEADLOCK DETECTED.\n";
    }
    else
    {
        std::cout
            << "\nUNSUPPORTED: A resource with multiple instances "
            << "participates in the graph.\n";
    }
}

int main()
{
    std::cout << "===== DEADLOCK DETECTION TEST =====\n";

    // TEST 1: Deadlock scenario

    ResourceAllocationGraph graph;

    graph.addProcess(0);
    graph.addProcess(1);
    graph.addProcess(2);

    graph.addResource(0, 1);
    graph.addResource(1, 1);
    graph.addResource(2, 3);

    graph.addAllocation(0, 0);
    graph.addAllocation(1, 1);

    graph.addRequest(0, 1);
    graph.addRequest(1, 0);

    std::cout << "\nTEST 1: Circular wait scenario\n";

    DetectionResult result =
        DeadlockDetector::detectSingleInstanceDeadlock(graph);

    printResult(result);

    // TEST 2: No deadlock scenario

    ResourceAllocationGraph graph2;

    graph2.addProcess(0);
    graph2.addProcess(1);

    graph2.addResource(0, 1);
    graph2.addResource(1, 1);

    graph2.addAllocation(0, 0);

    graph2.addRequest(1, 0);

    std::cout << "\nTEST 2: No circular wait\n";

    DetectionResult result2 =
        DeadlockDetector::detectSingleInstanceDeadlock(graph2);

    printResult(result2);

    return 0;
}
