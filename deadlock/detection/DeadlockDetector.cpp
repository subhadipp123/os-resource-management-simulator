
#include "DeadlockDetector.h"

#include <algorithm>
#include <functional>
#include <cstddef>
#include <limits>
#include <vector>

// --------------------------------------------------
// HELPER: FIND GRAPH ELEMENTS
// --------------------------------------------------

namespace
{
    std::size_t findProcessIndex(
        const std::vector<ProcessNode>& processes,
        int processId)
    {
        for (std::size_t i = 0; i < processes.size(); i++)
        {
            if (processes[i].id == processId)
                return i;
        }

        return processes.size();
    }

    std::size_t findResourceIndex(
        const std::vector<ResourceNode>& resources,
        int resourceId)
    {
        for (std::size_t i = 0; i < resources.size(); i++)
        {
            if (resources[i].id == resourceId)
                return i;
        }

        return resources.size();
    }

    bool containsEdge(
        const std::vector<GraphEdgeReference>& edges,
        int processId,
        int resourceId,
        EdgeType type)
    {
        return std::any_of(
            edges.begin(),
            edges.end(),
            [&](const GraphEdgeReference& edge)
            {
                return edge.processId == processId &&
                       edge.resourceId == resourceId &&
                       edge.type == type;
            }
        );
    }
}

// --------------------------------------------------
// SINGLE-INSTANCE DEADLOCK DETECTION
// --------------------------------------------------

DetectionResult DeadlockDetector::detectSingleInstanceDeadlock(
    const ResourceAllocationGraph& graph)
{
    const auto& processes = graph.getProcesses();
    const auto& resources = graph.getResources();
    const auto& edges = graph.getEdges();

    const std::size_t processCount = processes.size();

    DetectionResult result{
        DetectionStatus::NoDeadlock,
        {},
        {},
        {}
    };

    std::vector<std::vector<std::size_t>> waitForGraph(
        processCount
    );

    // Check that all participating resources have one instance.
    for (const auto& edge : edges)
    {
        std::size_t resourceIndex =
            findResourceIndex(resources, edge.resourceId);

        std::size_t processIndex =
            findProcessIndex(processes, edge.processId);

        if (resourceIndex == resources.size() ||
            processIndex == processCount ||
            edge.units <= 0)
        {
            result.status = DetectionStatus::InvalidGraph;
            return result;
        }

        if (resources[resourceIndex].totalInstances != 1)
        {
            result.status =
                DetectionStatus::UnsupportedMultipleInstances;

            return result;
        }

        if (edge.type == EdgeType::Allocation &&
            edge.units != 1)
        {
            result.status = DetectionStatus::InvalidGraph;
            return result;
        }
    }

    // Validate resource availability.
    for (const auto& resource : resources)
    {
        if (resource.totalInstances != 1 ||
            resource.availableInstances < 0 ||
            resource.availableInstances > resource.totalInstances)
        {
            result.status = DetectionStatus::InvalidGraph;
            return result;
        }
    }

    // Convert Resource Allocation Graph relationships
    // into Wait-For Graph relationships.
    for (const auto& requestEdge : edges)
    {
        if (requestEdge.type != EdgeType::Request)
            continue;

        std::size_t requestingProcess =
            findProcessIndex(processes, requestEdge.processId);

        if (requestingProcess == processCount)
        {
            result.status = DetectionStatus::InvalidGraph;
            return result;
        }

        for (const auto& allocationEdge : edges)
        {
            if (allocationEdge.type != EdgeType::Allocation)
                continue;

            if (allocationEdge.resourceId != requestEdge.resourceId)
                continue;

            if (requestEdge.units != 1)
            {
                result.status = DetectionStatus::InvalidGraph;
                return result;
            }

            std::size_t holdingProcess =
                findProcessIndex(processes, allocationEdge.processId);

            if (holdingProcess == processCount)
            {
                result.status = DetectionStatus::InvalidGraph;
                return result;
            }

            if (holdingProcess != requestingProcess)
            {
                waitForGraph[requestingProcess].push_back(
                    holdingProcess
                );
            }
        }
    }

    // DFS states:
    // 0 = unvisited
    // 1 = currently exploring
    // 2 = completely processed

    std::vector<int> state(processCount, 0);

    std::vector<std::size_t> recursionStack;

    bool cycleFound = false;

    std::function<void(std::size_t)> dfs =
        [&](std::size_t current)
    {
        state[current] = 1;
        recursionStack.push_back(current);

        for (std::size_t next : waitForGraph[current])
        {
            if (cycleFound)
                break;

            if (state[next] == 0)
            {
                dfs(next);
            }
            else if (state[next] == 1)
            {
                cycleFound = true;

                auto cycleStart = std::find(
                    recursionStack.begin(),
                    recursionStack.end(),
                    next
                );

                for (auto it = cycleStart;
                     it != recursionStack.end();
                     ++it)
                {
                    result.deadlockedProcessIds.push_back(
                        processes[*it].id
                    );
                }

                break;
            }
        }

        recursionStack.pop_back();
        state[current] = 2;
    };

    for (std::size_t i = 0; i < processCount; i++)
    {
        if (state[i] == 0)
        {
            dfs(i);
        }

        if (cycleFound)
            break;
    }

    if (cycleFound)
    {
        result.status = DetectionStatus::DeadlockDetected;
    }

    return result;
}

// --------------------------------------------------
// MULTIPLE-INSTANCE DEADLOCK DETECTION
// --------------------------------------------------

DetectionResult DeadlockDetector::detectMultipleInstanceDeadlock(
    const ResourceAllocationGraph& graph)
{
    const auto& processes = graph.getProcesses();
    const auto& resources = graph.getResources();
    const auto& edges = graph.getEdges();

    const std::size_t processCount = processes.size();
    const std::size_t resourceCount = resources.size();

    DetectionResult result{
        DetectionStatus::NoDeadlock,
        {},
        {},
        {}
    };

    // Work initially contains available resource instances.
    std::vector<int> work(resourceCount, 0);

    // Allocation[i][j] = resource instances held by process i.
    std::vector<std::vector<int>> allocation(
        processCount,
        std::vector<int>(resourceCount, 0)
    );

    // Request[i][j] = resource instances requested by process i.
    std::vector<std::vector<int>> request(
        processCount,
        std::vector<int>(resourceCount, 0)
    );

    // Validate resources and initialize Work.
    for (std::size_t j = 0; j < resourceCount; j++)
    {
        if (resources[j].totalInstances <= 0 ||
            resources[j].availableInstances < 0 ||
            resources[j].availableInstances >
                resources[j].totalInstances)
        {
            result.status = DetectionStatus::InvalidGraph;
            return result;
        }

        work[j] = resources[j].availableInstances;
    }

    // Map graph edges into Allocation and Request matrices.
    for (const auto& edge : edges)
    {
        std::size_t i =
            findProcessIndex(processes, edge.processId);

        std::size_t j =
            findResourceIndex(resources, edge.resourceId);

        if (i == processCount ||
            j == resourceCount ||
            edge.units <= 0)
        {
            result.status = DetectionStatus::InvalidGraph;
            return result;
        }

        int& target =
            (edge.type == EdgeType::Allocation)
                ? allocation[i][j]
                : request[i][j];

        // Prevent integer overflow.
        if (target >
            std::numeric_limits<int>::max() - edge.units)
        {
            result.status = DetectionStatus::InvalidGraph;
            return result;
        }

        target += edge.units;
    }

    // Validate resource accounting.
    for (std::size_t j = 0; j < resourceCount; j++)
    {
        long long allocatedTotal = 0;

        for (std::size_t i = 0; i < processCount; i++)
        {
            allocatedTotal += allocation[i][j];
        }

        if (allocatedTotal + work[j] !=
            resources[j].totalInstances)
        {
            result.status = DetectionStatus::InvalidGraph;
            return result;
        }
    }

    // Finish[i] indicates whether process i can finish.
    std::vector<bool> finish(processCount, false);

    // Processes holding no resources are initially finished.
    for (std::size_t i = 0; i < processCount; i++)
    {
        bool holdsResource = false;

        for (std::size_t j = 0; j < resourceCount; j++)
        {
            if (allocation[i][j] > 0)
            {
                holdsResource = true;
                break;
            }
        }

        finish[i] = !holdsResource;
    }

    // Repeatedly find a process whose request can be satisfied.
    bool progress = true;

    while (progress)
    {
        progress = false;

        for (std::size_t i = 0; i < processCount; i++)
        {
            if (finish[i])
                continue;

            bool canFinish = true;

            for (std::size_t j = 0; j < resourceCount; j++)
            {
                if (request[i][j] > work[j])
                {
                    canFinish = false;
                    break;
                }
            }

            if (!canFinish)
                continue;

            // Assume the process completes and releases resources.
            for (std::size_t j = 0; j < resourceCount; j++)
            {
                work[j] += allocation[i][j];
            }

            finish[i] = true;
            progress = true;
        }
    }

    // Identify processes remaining unfinished.
    for (std::size_t i = 0; i < processCount; i++)
    {
        if (!finish[i])
        {
            result.deadlockedProcessIds.push_back(
                processes[i].id
            );
        }
    }

    if (result.deadlockedProcessIds.empty())
    {
        result.status = DetectionStatus::NoDeadlock;
        return result;
    }

    result.status = DetectionStatus::DeadlockDetected;

    // Identify requests that remain blocked.
    for (std::size_t i = 0; i < processCount; i++)
    {
        if (finish[i])
            continue;

        for (std::size_t j = 0; j < resourceCount; j++)
        {
            if (request[i][j] > work[j])
            {
                if (!containsEdge(
                    result.blockedRequestEdges,
                    processes[i].id,
                    resources[j].id,
                    EdgeType::Request))
                {
                    result.blockedRequestEdges.push_back({
                        processes[i].id,
                        resources[j].id,
                        EdgeType::Request
                    });
                }

                // Identify allocations of this resource
                // held by unfinished processes.
                for (std::size_t holder = 0;
                     holder < processCount;
                     holder++)
                {
                    if (finish[holder] ||
                        allocation[holder][j] == 0)
                    {
                        continue;
                    }

                    if (!containsEdge(
                        result.blockingAllocationEdges,
                        processes[holder].id,
                        resources[j].id,
                        EdgeType::Allocation))
                    {
                        result.blockingAllocationEdges.push_back({
                            processes[holder].id,
                            resources[j].id,
                            EdgeType::Allocation
                        });
                    }
                }
            }
        }
    }

    return result;
}
