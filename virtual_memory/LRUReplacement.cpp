#include "LRUReplacement.h"

int LRUReplacement::selectVictimFrame(
    const std::vector<Frame>& frames
) const
{
    int victimFrame = -1;

    unsigned long long oldestUsedTime = 0;

    for (int i = 0; i < static_cast<int>(frames.size()); i++)
    {
        if (!frames[i].occupied)
        {
            continue;
        }

        if (victimFrame == -1 ||
            frames[i].lastUsedTime < oldestUsedTime)
        {
            victimFrame = i;
            oldestUsedTime = frames[i].lastUsedTime;
        }
    }

    return victimFrame;
}