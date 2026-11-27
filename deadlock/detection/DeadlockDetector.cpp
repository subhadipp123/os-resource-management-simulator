
#include "DeadlockDetector.h"

#include <algorithm>
#include <functional>
#include <cstddef>
#include <vector>

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
        {}
    };

    std::vector<std::vector<std::size_t>> waitForGraph(
        processCount
    );

    auto findProcessIndex = [&](int processId)
    {
        for (std::size_t i = 0; i < processCount; i++)
        {
            if (processes[i].id == processId)
                return i;
        }

        return processCount;
    };

    auto findResource = [&](int resourceId)
    {
        return std::find_if(
            resources.begin(),
            resources.end(),
            [resourceId](const ResourceNode& resource)
            {
                return resource.id == resourceId;
            }
        );
    };

    // Reject participating resources with multiple instances.
    for (const auto& edge : edges)
    {
        auto resource = findResource(edge.resourceId);

        if (resource == resources.end())
            continue;

        if (resource->totalInstances != 1)
        {
            result.status =
                DetectionStatus::UnsupportedMultipleInstances;

            return result;
        }
    }

    // Convert RAG relationships into WFG relationships.
    for (const auto& requestEdge : edges)
    {
        if (requestEdge.type != EdgeType::Request)
            continue;

        std::size_t requestingProcess =
            findProcessIndex(requestEdge.processId);

        if (requestingProcess == processCount)
            continue;

        for (const auto& allocationEdge : edges)
        {
            if (allocationEdge.type != EdgeType::Allocation)
                continue;

            if (allocationEdge.resourceId != requestEdge.resourceId)
                continue;

            std::size_t holdingProcess =
                findProcessIndex(allocationEdge.processId);

            if (holdingProcess == processCount)
                continue;

            waitForGraph[requestingProcess].push_back(
                holdingProcess
            );
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
                return;

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
        {}
    };

    // Work initially contains the available instances.
    std::vector<int> work(resourceCount, 0);

    // Allocation[i][j] = instances of resource j held by process i.
    std::vector<std::vector<int>> allocation(
        processCount,
        std::vector<int>(resourceCount, 0)
    );

    // Request[i][j] = instances of resource j requested by process i.
    std::vector<std::vector<int>> request(
        processCount,
        std::vector<int>(resourceCount, 0)
    );

    // Build Work from Available.
    for (std::size_t j = 0; j < resourceCount; j++)
    {
        work[j] = resources[j].availableInstances;

        if (work[j] < 0 ||
            work[j] > resources[j].totalInstances)
        {
            return result;
        }
    }

    // Map graph edges into the matrices.
    for (const auto& edge : edges)
    {
        auto processIt = std::find_if(
            processes.begin(),
            processes.end(),
            [&](const ProcessNode& process)
            {
                return process.id == edge.processId;
            }
        );

        auto resourceIt = std::find_if(
            resources.begin(),
            resources.end(),
            [&](const ResourceNode& resource)
            {
                return resource.id == edge.resourceId;
            }
        );

        if (processIt == processes.end() ||
            resourceIt == resources.end() ||
            edge.units <= 0)
        {
            return result;
        }

        std::size_t i = static_cast<std::size_t>(
            std::distance(processes.begin(), processIt)
        );

        std::size_t j = static_cast<std::size_t>(
            std::distance(resources.begin(), resourceIt)
        );

        if (edge.type == EdgeType::Allocation)
        {
            allocation[i][j] += edge.units;
        }
        else
        {
            request[i][j] += edge.units;
        }
    }

    // Validate resource accounting.
    for (std::size_t j = 0; j < resourceCount; j++)
    {
        int allocatedTotal = 0;

        for (std::size_t i = 0; i < processCount; i++)
        {
            allocatedTotal += allocation[i][j];
        }

        if (allocatedTotal + work[j] !=
            resources[j].totalInstances)
        {
            return result;
        }
    }

    // Finish[i] indicates whether process i can finish.
    std::vector<bool> finish(processCount, false);

    // Processes holding no resources are initially finished
    // according to the standard detection algorithm.
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

    // Remaining unfinished processes are deadlocked.
    for (std::size_t i = 0; i < processCount; i++)
    {
        if (!finish[i])
        {
            result.deadlockedProcessIds.push_back(
                processes[i].id
            );
        }
    }

    if (!result.deadlockedProcessIds.empty())
    {
        result.status = DetectionStatus::DeadlockDetected;
    }

    return result;
}
