
#ifndef FRAME_H
#define FRAME_H

struct Frame
{
    int frameNumber;
    int pageNumber;
    bool occupied;
    unsigned long long loadTime;
    unsigned long long lastUsedTime;

    Frame()
        : frameNumber(-1),
          pageNumber(-1),
          occupied(false),
          loadTime(0),
          lastUsedTime(0)
    {
    }
};

#endif
