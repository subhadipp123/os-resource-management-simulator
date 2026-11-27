
#ifndef DEADLOCK_DETECTOR_H
#define DEADLOCK_DETECTOR_H

#include "../graph/ResourceAllocationGraph.h"

#include <vector>

enum class DetectionStatus
{
    NoDeadlock,
    DeadlockDetected,
    UnsupportedMultipleInstances
};

struct DetectionResult
{
    DetectionStatus status;

    std::vector<int> deadlockedProcessIds;
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
