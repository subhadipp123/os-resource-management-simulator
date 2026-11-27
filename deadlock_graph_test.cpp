
#include "deadlock/graph/ResourceAllocationGraph.h"

int main()
{
    ResourceAllocationGraph graph;

    graph.addProcess(0);
    graph.addProcess(1);

    graph.addResource(0, 1);
    graph.addResource(1, 1);

    graph.addAllocation(0, 0);
    graph.addAllocation(1, 1);

    graph.addRequest(0, 1);
    graph.addRequest(1, 0);

    graph.displayGraph();

    return 0;
}
