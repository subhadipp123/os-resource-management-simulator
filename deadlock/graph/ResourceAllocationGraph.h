
#ifndef RESOURCE_ALLOCATION_GRAPH_H
#define RESOURCE_ALLOCATION_GRAPH_H

#include "GraphTypes.h"

#include <vector>

class ResourceAllocationGraph
{
private:
    std::vector<ProcessNode> processes;
    std::vector<ResourceNode> resources;
    std::vector<GraphEdge> edges;

public:
    bool addProcess(int processId);

    bool addResource(
        int resourceId,
        int totalInstances
    );

    bool addRequest(
        int processId,
        int resourceId,
        int units = 1
    );

    bool addAllocation(
        int processId,
        int resourceId,
        int units = 1
    );

    bool removeRequest(
        int processId,
        int resourceId,
        int units = 1
    );

    bool removeAllocation(
        int processId,
        int resourceId,
        int units = 1
    );

    void displayGraph() const;

    const std::vector<ProcessNode>& getProcesses() const
    {
        return processes;
    }

    const std::vector<ResourceNode>& getResources() const
    {
        return resources;
    }

    const std::vector<GraphEdge>& getEdges() const
    {
        return edges;
    }
};

#endif
