
#include "ResourceAllocationGraph.h"

#include <iostream>
#include <algorithm>

// Find a process by ID
static auto findProcess(
    std::vector<ProcessNode>& processes,
    int processId)
{
    return std::find_if(
        processes.begin(),
        processes.end(),
        [processId](const ProcessNode& process)
        {
            return process.id == processId;
        }
    );
}

// Find a resource by ID
static auto findResource(
    std::vector<ResourceNode>& resources,
    int resourceId)
{
    return std::find_if(
        resources.begin(),
        resources.end(),
        [resourceId](const ResourceNode& resource)
        {
            return resource.id == resourceId;
        }
    );
}

// Add a process
bool ResourceAllocationGraph::addProcess(int processId)
{
    if (processId < 0)
        return false;

    if (findProcess(processes, processId) != processes.end())
        return false;

    processes.push_back({
        processId,
        "P" + std::to_string(processId)
    });

    return true;
}

// Add a resource
bool ResourceAllocationGraph::addResource(
    int resourceId,
    int totalInstances)
{
    if (resourceId < 0 || totalInstances <= 0)
        return false;

    if (findResource(resources, resourceId) != resources.end())
        return false;

    resources.push_back({
        resourceId,
        "R" + std::to_string(resourceId),
        totalInstances,
        totalInstances
    });

    return true;
}

// Add or increase a request edge: P -> R
bool ResourceAllocationGraph::addRequest(
    int processId,
    int resourceId,
    int units)
{
    if (units <= 0)
        return false;

    if (findProcess(processes, processId) == processes.end())
        return false;

    if (findResource(resources, resourceId) == resources.end())
        return false;

    auto existingEdge = std::find_if(
        edges.begin(),
        edges.end(),
        [processId, resourceId](const GraphEdge& edge)
        {
            return edge.processId == processId &&
                   edge.resourceId == resourceId &&
                   edge.type == EdgeType::Request;
        }
    );

    if (existingEdge != edges.end())
    {
        if (existingEdge->units > 
            std::numeric_limits<int>::max() - units)
        {
            return false;
        }

        existingEdge->units += units;
    }
    else
    {
        edges.push_back({
            processId,
            resourceId,
            EdgeType::Request,
            units
        });
    }

    return true;
}

// Add or increase an allocation edge: R -> P
bool ResourceAllocationGraph::addAllocation(
    int processId,
    int resourceId,
    int units)
{
    if (units <= 0)
        return false;

    if (findProcess(processes, processId) == processes.end())
        return false;

    auto resource = findResource(resources, resourceId);

    if (resource == resources.end())
        return false;

    if (resource->availableInstances < units)
        return false;

    auto existingEdge = std::find_if(
        edges.begin(),
        edges.end(),
        [processId, resourceId](const GraphEdge& edge)
        {
            return edge.processId == processId &&
                   edge.resourceId == resourceId &&
                   edge.type == EdgeType::Allocation;
        }
    );

    if (existingEdge != edges.end())
    {
        if (existingEdge->units >
            std::numeric_limits<int>::max() - units)
        {
            return false;
        }

        existingEdge->units += units;
    }
    else
    {
        edges.push_back({
            processId,
            resourceId,
            EdgeType::Allocation,
            units
        });
    }

    resource->availableInstances -= units;

    return true;
}

// Remove some or all units from a request edge
bool ResourceAllocationGraph::removeRequest(
    int processId,
    int resourceId,
    int units)
{
    if (units <= 0)
        return false;

    auto it = std::find_if(
        edges.begin(),
        edges.end(),
        [processId, resourceId](const GraphEdge& edge)
        {
            return edge.processId == processId &&
                   edge.resourceId == resourceId &&
                   edge.type == EdgeType::Request;
        }
    );

    if (it == edges.end() || it->units < units)
        return false;

    it->units -= units;

    if (it->units == 0)
        edges.erase(it);

    return true;
}

// Release allocated resource units
bool ResourceAllocationGraph::removeAllocation(
    int processId,
    int resourceId,
    int units)
{
    if (units <= 0)
        return false;

    auto it = std::find_if(
        edges.begin(),
        edges.end(),
        [processId, resourceId](const GraphEdge& edge)
        {
            return edge.processId == processId &&
                   edge.resourceId == resourceId &&
                   edge.type == EdgeType::Allocation;
        }
    );

    if (it == edges.end() || it->units < units)
        return false;

    auto resource = findResource(resources, resourceId);

    if (resource == resources.end())
        return false;

    it->units -= units;

    resource->availableInstances += units;

    if (it->units == 0)
        edges.erase(it);

    return true;
}

// Display graph information
void ResourceAllocationGraph::displayGraph() const
{
    std::cout << "\n===== RESOURCE ALLOCATION GRAPH =====\n";

    std::cout << "\nProcesses:\n";

    for (const auto& process : processes)
    {
        std::cout << process.name << '\n';
    }

    std::cout << "\nResources:\n";

    for (const auto& resource : resources)
    {
        std::cout
            << resource.name
            << " | Total: " << resource.totalInstances
            << " | Available: " << resource.availableInstances
            << '\n';
    }

    std::cout << "\nEdges:\n";

    for (const auto& edge : edges)
    {
        if (edge.type == EdgeType::Request)
        {
            std::cout
                << "P" << edge.processId
                << " -> R" << edge.resourceId
                << " (Request: " << edge.units << ")\n";
        }
        else
        {
            std::cout
                << "R" << edge.resourceId
                << " -> P" << edge.processId
                << " (Allocation: " << edge.units << ")\n";
        }
    }
}
