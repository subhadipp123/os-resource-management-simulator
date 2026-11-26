
#ifndef PAGE_REPLACEMENT_TYPES_H
#define PAGE_REPLACEMENT_TYPES_H

#include <vector>

struct PageReplacementStep
{
    int referencePage;
    std::vector<int> frameContents;
    bool pageHit;
    int replacedPage;
    int targetFrame;
};

struct PageReplacementReport
{
    std::vector<PageReplacementStep> steps;
    int pageHits = 0;
    int pageFaults = 0;
    double hitRatio = 0.0;
    double faultRate = 0.0;
};

#endif
