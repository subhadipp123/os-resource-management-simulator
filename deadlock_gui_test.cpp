
#include "deadlock/graph/GraphRenderer.h"

#include <iostream>

int main()
{
    ResourceAllocationGraph graph;

    // Create processes
    graph.addProcess(0);
    graph.addProcess(1);
    graph.addProcess(2);

    // Create resources
    graph.addResource(0, 1);
    graph.addResource(1, 1);
    graph.addResource(2, 3);

    // Allocate resources
    graph.addAllocation(0, 0);
    graph.addAllocation(1, 1);

    // Create pending requests
    graph.addRequest(0, 1);
    graph.addRequest(1, 0);

    std::cout << "Starting Deadlock Graph Visualization...\n";

    GraphRenderer renderer(graph);

    renderer.run();

    return 0;
}
