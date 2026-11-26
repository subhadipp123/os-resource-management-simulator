
#include "OptimalReplacement.h"

int OptimalReplacement::selectVictimFrame(
    const std::vector<Frame>& frames,
    const std::vector<int>& referenceString,
    int currentIndex
) const
{
    int victimFrame = -1;
    int farthestNextUse = -1;

    for (int i = 0; i < static_cast<int>(frames.size()); i++)
    {
        if (!frames[i].occupied)
            continue;

        int nextUse = -1;

        for (int j = currentIndex + 1;
             j < static_cast<int>(referenceString.size());
             j++)
        {
            if (referenceString[j] == frames[i].pageNumber)
            {
                nextUse = j;
                break;
            }
        }

        // A page that is never used again is the best victim.
        if (nextUse == -1)
            return i;

        if (nextUse > farthestNextUse)
        {
            farthestNextUse = nextUse;
            victimFrame = i;
        }
    }

    return victimFrame;
}
