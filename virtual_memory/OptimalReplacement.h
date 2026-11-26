
#ifndef OPTIMAL_REPLACEMENT_H
#define OPTIMAL_REPLACEMENT_H

#include <vector>
#include "Frame.h"

class OptimalReplacement
{
public:
    int selectVictimFrame(
        const std::vector<Frame>& frames,
        const std::vector<int>& referenceString,
        int currentIndex
    ) const;
};

#endif
