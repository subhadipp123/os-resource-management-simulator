#ifndef LRU_REPLACEMENT_H
#define LRU_REPLACEMENT_H

#include <vector>
#include "Frame.h"

class LRUReplacement
{
public:
    int selectVictimFrame(
        const std::vector<Frame>& frames
    ) const;
};

#endif