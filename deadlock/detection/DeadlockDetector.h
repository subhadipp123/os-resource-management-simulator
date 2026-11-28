
#ifndef DEADLOCK_DETECTOR_H
#define DEADLOCK_DETECTOR_H

#include "../graph/ResourceAllocationGraph.h"

#include <vector>

enum class DetectionStatus
{
    NoDeadlock,
    DeadlockDetected,
    UnsupportedMultipleInstances,
    InvalidGraph
};

// Identifies a specific edge in the Resource Allocation Graph.
struct GraphEdgeReference
{
    int processId;
    int resourceId;
    EdgeType type;
};

struct DetectionResult
{
    DetectionStatus status;

    // Processes identified as deadlocked.
    std::vector<int> deadlockedProcessIds;

    // Requests that remain blocked.
    std::vector<GraphEdgeReference> blockedRequestEdges;

    // Allocations held by unfinished processes
    // for resources involved in blocked requests.
    std::vector<GraphEdgeReference> blockingAllocationEdges;
};

class DeadlockDetector
{
public:
    static DetectionResult detectSingleInstanceDeadlock(
        const ResourceAllocationGraph& graph
    );

    static DetectionResult detectMultipleInstanceDeadlock(
        const ResourceAllocationGraph& graph
    );
};

#endif
