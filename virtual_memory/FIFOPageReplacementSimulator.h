
#ifndef FIFO_PAGE_REPLACEMENT_SIMULATOR_H
#define FIFO_PAGE_REPLACEMENT_SIMULATOR_H

#include <vector>
#include "PageReplacementTypes.h"

class FIFOPageReplacementSimulator
{
public:
    PageReplacementReport run(
        const std::vector<int>& referenceString,
        int totalFrames
    ) const;
};

#endif
