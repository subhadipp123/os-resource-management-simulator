
#include "OptimalPageReplacementSimulator.h"

#include <stdexcept>

#include "OptimalReplacement.h"
#include "PageReferenceMapper.h"
#include "VirtualMemoryManager.h"

PageReplacementReport OptimalPageReplacementSimulator::run(
    const std::vector<int>& referenceString,
    int totalFrames
) const
{
    if (totalFrames <= 0)
    {
        throw std::invalid_argument(
            "Number of frames must be greater than zero."
        );
    }

    PageReferenceMapping mapping =
        mapPageReferences(referenceString);

    const std::vector<int>& references =
        mapping.compactReferences;

    VirtualMemoryManager manager(
        static_cast<int>(mapping.originalPages.size()),
        totalFrames
    );

    OptimalReplacement optimal;
    PageReplacementReport report;

    for (std::size_t i = 0; i < references.size(); i++)
    {
        int compactPage = references[i];
        int originalPage = referenceString[i];

        PageReplacementStep step{};

        step.referencePage = originalPage;
        step.pageHit = false;
        step.replacedPage = -1;
        step.targetFrame = -1;

        PageAccessResult result =
            manager.accessPage(compactPage);

        if (result == PageAccessResult::PAGE_HIT)
        {
            step.pageHit = true;
            report.pageHits++;
        }
        else if (result == PageAccessResult::PAGE_FAULT_HANDLED)
        {
            report.pageFaults++;

            const auto& frames = manager.getFrames();

            for (const Frame& frame : frames)
            {
                if (frame.occupied &&
                    frame.pageNumber == compactPage)
                {
                    step.targetFrame = frame.frameNumber;
                    break;
                }
            }
        }
        else if (result == PageAccessResult::NO_FREE_FRAME)
        {
            report.pageFaults++;

            const auto& frames = manager.getFrames();

            int victimFrame = optimal.selectVictimFrame(
                frames,
                references,
                static_cast<int>(i)
            );

            if (victimFrame == -1)
            {
                throw std::runtime_error(
                    "Optimal algorithm could not find a victim frame."
                );
            }

            step.targetFrame = victimFrame;

            step.replacedPage =
                mapping.originalPages[
                    frames[victimFrame].pageNumber
                ];

            if (!manager.replacePage(compactPage, victimFrame))
            {
                throw std::runtime_error(
                    "Optimal page replacement failed."
                );
            }
        }
        else
        {
            throw std::runtime_error(
                "Invalid page encountered during simulation."
            );
        }

        step.frameContents.assign(totalFrames, -1);

        for (const Frame& frame : manager.getFrames())
        {
            if (frame.occupied)
            {
                step.frameContents[frame.frameNumber] =
                    mapping.originalPages[frame.pageNumber];
            }
        }

        report.steps.push_back(step);
    }

    int totalReferences =
        static_cast<int>(referenceString.size());

    report.hitRatio =
        100.0 * report.pageHits / totalReferences;

    report.faultRate =
        100.0 * report.pageFaults / totalReferences;

    return report;
}
