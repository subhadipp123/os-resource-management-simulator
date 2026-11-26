
#include "FIFOReplacement.h"

int FIFOReplacement::selectVictimFrame(
    const std::vector<Frame>& frames
) const
{
    int victimFrame = -1;
    unsigned long long oldestLoadTime = 0;

    for (int i = 0; i < static_cast<int>(frames.size()); i++)
    {
        if (!frames[i].occupied)
        {
            continue;
        }

        if (victimFrame == -1 ||
            frames[i].loadTime < oldestLoadTime)
        {
            victimFrame = i;
            oldestLoadTime = frames[i].loadTime;
        }
    }

    return victimFrame;
}
