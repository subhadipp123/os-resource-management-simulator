
#ifndef FIFO_REPLACEMENT_H
#define FIFO_REPLACEMENT_H

#include <vector>
#include "Frame.h"

class FIFOReplacement
{
public:
    int selectVictimFrame(const std::vector<Frame>& frames) const;
};

#endif
