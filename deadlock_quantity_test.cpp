
#include "deadlock/graph/ResourceAllocationGraph.h"

#include <iostream>

int main()
{
    ResourceAllocationGraph graph;

    graph.addProcess(0);
    graph.addProcess(1);
    graph.addProcess(2);

    graph.addResource(0, 5);

    // P0 receives 2 instances.
    graph.addAllocation(0, 0, 2);

    // P1 receives 1 instance.
    graph.addAllocation(1, 0, 1);

    // P2 requests 3 instances.
    graph.addRequest(2, 0, 3);

    std::cout << "INITIAL RESOURCE STATE\n";

    graph.displayGraph();

    // Release one instance from P0.
    std::cout << "\nReleasing 1 instance from P0...\n";

    graph.removeAllocation(0, 0, 1);

    graph.displayGraph();

    // Remove one requested unit from P2.
    std::cout << "\nRemoving 1 requested unit from P2...\n";

    graph.removeRequest(2, 0, 1);

    graph.displayGraph();

    return 0;
}
