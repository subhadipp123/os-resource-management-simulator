
#ifndef OPTIMAL_PAGE_REPLACEMENT_SIMULATOR_H
#define OPTIMAL_PAGE_REPLACEMENT_SIMULATOR_H

#include <vector>
#include "PageReplacementTypes.h"

class OptimalPageReplacementSimulator
{
public:
    PageReplacementReport run(
        const std::vector<int>& referenceString,
        int totalFrames
    ) const;
};

#endif
