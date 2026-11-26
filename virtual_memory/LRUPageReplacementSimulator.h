
#ifndef LRU_PAGE_REPLACEMENT_SIMULATOR_H
#define LRU_PAGE_REPLACEMENT_SIMULATOR_H

#include <vector>
#include "PageReplacementTypes.h"

class LRUPageReplacementSimulator
{
public:
    PageReplacementReport run(
        const std::vector<int>& referenceString,
        int totalFrames
    ) const;
};

#endif
