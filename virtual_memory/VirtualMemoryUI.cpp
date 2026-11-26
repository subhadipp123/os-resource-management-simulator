
#include "VirtualMemoryUI.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <exception>

#include "FIFOPageReplacementSimulator.h"
#include "LRUPageReplacementSimulator.h"
#include "OptimalPageReplacementSimulator.h"

namespace
{
    bool readPositiveInteger(
        const std::string& prompt,
        int& value
    )
    {
        std::cout << prompt;

        if (!(std::cin >> value))
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout
                << "Invalid input. Enter a positive integer.\n";

            return false;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        if (value <= 0)
        {
            std::cout
                << "Value must be greater than zero.\n";

            return false;
        }

        return true;
    }

    bool parseReferenceString(
        const std::string& input,
        std::vector<int>& references
    )
    {
        std::string normalized = input;

        for (char& ch : normalized)
        {
            if (ch == ',')
                ch = ' ';
        }

        std::istringstream stream(normalized);

        int page;

        while (stream >> page)
        {
            if (page < 0)
                return false;

            references.push_back(page);
        }

        if (!stream.eof() || references.empty())
        {
            references.clear();
            return false;
        }

        return true;
    }

    bool readSimulationInput(
        std::vector<int>& referenceString,
        int& totalFrames
    )
    {
        std::string input;

        std::cout
            << "\nEnter reference string "
            << "(comma or space separated): ";

        std::getline(std::cin, input);

        if (!parseReferenceString(input, referenceString))
        {
            std::cout
                << "Invalid reference string. "
                << "Example: 1, 2, 3, 2, 4.\n";

            return false;
        }

        if (!readPositiveInteger(
                "Enter number of frames: ",
                totalFrames
            ))
        {
            return false;
        }

        return true;
    }

    void displayReport(
        const std::string& algorithmName,
        const std::vector<int>& referenceString,
        int totalFrames,
        const PageReplacementReport& report
    )
    {
        std::cout
            << "\n" << algorithmName
            << " PAGE REPLACEMENT - FRAME-BY-FRAME TRACE\n";

        std::cout
            << "================================================\n";

        std::cout
            << std::left
            << std::setw(12) << "Reference";

        for (int i = 0; i < totalFrames; i++)
        {
            std::cout
                << std::setw(8)
                << ("F" + std::to_string(i + 1));
        }

        std::cout
            << std::setw(12) << "Status"
            << "Replacement Details\n";

        std::cout
            << std::string(
                12 + totalFrames * 8 + 34,
                '-'
            )
            << '\n';

        for (const PageReplacementStep& step : report.steps)
        {
            std::cout
                << std::left
                << std::setw(12)
                << step.referencePage;

            for (int page : step.frameContents)
            {
                std::string cell =
                    (page == -1)
                    ? "-"
                    : std::to_string(page);

                std::cout
                    << std::setw(8)
                    << cell;
            }

            std::cout
                << std::setw(12)
                << (step.pageHit ? "HIT" : "FAULT");

            if (step.pageHit)
            {
                std::cout
                    << "Page " << step.referencePage
                    << " already exists in memory.\n";
            }
            else if (step.replacedPage == -1)
            {
                std::cout
                    << "Loaded Page "
                    << step.referencePage
                    << " into Frame "
                    << step.targetFrame + 1
                    << ".\n";
            }
            else
            {
                std::cout
                    << "Replaced Page "
                    << step.replacedPage
                    << " with Page "
                    << step.referencePage
                    << " in Frame "
                    << step.targetFrame + 1
                    << ".\n";
            }
        }

        std::cout << "\nPERFORMANCE SUMMARY\n";
        std::cout << "--------------------\n";

        std::cout
            << "Total References: "
            << referenceString.size()
            << '\n';

        std::cout
            << "Page Hits: "
            << report.pageHits
            << '\n';

        std::cout
            << "Page Faults: "
            << report.pageFaults
            << '\n';

        std::cout
            << std::fixed
            << std::setprecision(2);

        std::cout
            << "Hit Ratio: "
            << report.hitRatio
            << "%\n";

        std::cout
            << "Page Fault Rate: "
            << report.faultRate
            << "%\n";
    }

    void displayComparison(
        const PageReplacementReport& fifo,
        const PageReplacementReport& lru,
        const PageReplacementReport& optimal
    )
    {
        std::cout
            << "\n\n========================================\n";

        std::cout
            << "       ALGORITHM COMPARISON\n";

        std::cout
            << "========================================\n";

        std::cout
            << std::left
            << std::setw(15) << "Algorithm"
            << std::setw(15) << "Page Hits"
            << std::setw(15) << "Page Faults"
            << std::setw(15) << "Hit Ratio"
            << "Fault Rate\n";

        std::cout << std::string(75, '-') << '\n';

        std::cout
            << std::fixed
            << std::setprecision(2);

        std::cout
            << std::left
            << std::setw(15) << "FIFO"
            << std::setw(15) << fifo.pageHits
            << std::setw(15) << fifo.pageFaults
            << std::setw(15) << fifo.hitRatio
            << fifo.faultRate << "%\n";

        std::cout
            << std::left
            << std::setw(15) << "LRU"
            << std::setw(15) << lru.pageHits
            << std::setw(15) << lru.pageFaults
            << std::setw(15) << lru.hitRatio
            << lru.faultRate << "%\n";

        std::cout
            << std::left
            << std::setw(15) << "Optimal"
            << std::setw(15) << optimal.pageHits
            << std::setw(15) << optimal.pageFaults
            << std::setw(15) << optimal.hitRatio
            << optimal.faultRate << "%\n";

        int minimumFaults = fifo.pageFaults;

        if (lru.pageFaults < minimumFaults)
            minimumFaults = lru.pageFaults;

        if (optimal.pageFaults < minimumFaults)
            minimumFaults = optimal.pageFaults;

        std::cout
            << "\nBEST RESULT FOR THIS REFERENCE STRING\n";

        if (fifo.pageFaults == minimumFaults)
            std::cout << "FIFO ";

        if (lru.pageFaults == minimumFaults)
            std::cout << "LRU ";

        if (optimal.pageFaults == minimumFaults)
            std::cout << "Optimal ";

        std::cout
            << "with " << minimumFaults
            << " page faults.\n";
    }

    void runSingleAlgorithm(
        const std::vector<int>& referenceString,
        int totalFrames,
        int choice
    )
    {
        if (choice == 1)
        {
            FIFOPageReplacementSimulator simulator;

            PageReplacementReport report =
                simulator.run(referenceString, totalFrames);

            displayReport(
                "FIFO",
                referenceString,
                totalFrames,
                report
            );
        }
        else if (choice == 2)
        {
            LRUPageReplacementSimulator simulator;

            PageReplacementReport report =
                simulator.run(referenceString, totalFrames);

            displayReport(
                "LRU",
                referenceString,
                totalFrames,
                report
            );
        }
        else if (choice == 3)
        {
            OptimalPageReplacementSimulator simulator;

            PageReplacementReport report =
                simulator.run(referenceString, totalFrames);

            displayReport(
                "OPTIMAL",
                referenceString,
                totalFrames,
                report
            );
        }
    }

    void runAllComparisons(
        const std::vector<int>& referenceString,
        int totalFrames
    )
    {
        FIFOPageReplacementSimulator fifoSimulator;
        LRUPageReplacementSimulator lruSimulator;
        OptimalPageReplacementSimulator optimalSimulator;

        PageReplacementReport fifo =
            fifoSimulator.run(referenceString, totalFrames);

        PageReplacementReport lru =
            lruSimulator.run(referenceString, totalFrames);

        PageReplacementReport optimal =
            optimalSimulator.run(referenceString, totalFrames);

        displayReport(
            "FIFO",
            referenceString,
            totalFrames,
            fifo
        );

        displayReport(
            "LRU",
            referenceString,
            totalFrames,
            lru
        );

        displayReport(
            "OPTIMAL",
            referenceString,
            totalFrames,
            optimal
        );

        displayComparison(fifo, lru, optimal);
    }

    void checkBeladysAnomaly(
        const std::vector<int>& referenceString,
        int totalFrames
    )
    {
        if (totalFrames == std::numeric_limits<int>::max())
        {
            std::cout
                << "Cannot test with the maximum integer frame count.\n";

            return;
        }

        int increasedFrames = totalFrames + 1;

        FIFOPageReplacementSimulator fifoSimulator;
        LRUPageReplacementSimulator lruSimulator;
        OptimalPageReplacementSimulator optimalSimulator;

        PageReplacementReport fifoBefore =
            fifoSimulator.run(referenceString, totalFrames);

        PageReplacementReport fifoAfter =
            fifoSimulator.run(referenceString, increasedFrames);

        PageReplacementReport lruBefore =
            lruSimulator.run(referenceString, totalFrames);

        PageReplacementReport lruAfter =
            lruSimulator.run(referenceString, increasedFrames);

        PageReplacementReport optimalBefore =
            optimalSimulator.run(referenceString, totalFrames);

        PageReplacementReport optimalAfter =
            optimalSimulator.run(referenceString, increasedFrames);

        auto displayResult = [&](
            const std::string& name,
            int faultsBefore,
            int faultsAfter
        )
        {
            std::cout << "\n" << name << '\n';

            std::cout
                << "Page Faults ("
                << totalFrames
                << " frames): "
                << faultsBefore
                << '\n';

            std::cout
                << "Page Faults ("
                << increasedFrames
                << " frames): "
                << faultsAfter
                << '\n';

            if (faultsAfter > faultsBefore)
            {
                std::cout
                    << "RESULT: Belady's Anomaly observed!\n";
            }
            else
            {
                std::cout
                    << "RESULT: Belady's Anomaly not observed.\n";
            }
        };

        std::cout
            << "\n========================================\n";

        std::cout
            << "       BELADY'S ANOMALY CHECK\n";

        std::cout
            << "========================================\n";

        displayResult(
            "FIFO",
            fifoBefore.pageFaults,
            fifoAfter.pageFaults
        );

        displayResult(
            "LRU",
            lruBefore.pageFaults,
            lruAfter.pageFaults
        );

        displayResult(
            "OPTIMAL",
            optimalBefore.pageFaults,
            optimalAfter.pageFaults
        );

        std::cout
            << "\n========================================\n";

        std::cout
            << "       ANOMALY CHECK COMPLETED\n";

        std::cout
            << "========================================\n";
    }
}

void runVirtualMemoryMenu()
{
    int choice = -1;

    do
    {
        std::cout
            << "\n========================================\n";

        std::cout
            << "       VIRTUAL MEMORY MANAGEMENT\n";

        std::cout
            << "========================================\n";

        std::cout
            << "1. Run a Single Page Replacement Algorithm\n";

        std::cout
            << "2. Compare All Page Replacement Algorithms\n";

        std::cout
            << "3. Check Belady's Anomaly\n";

        std::cout
            << "4. Back to Main Menu\n";

        std::cout << "Enter choice: ";

        if (!(std::cin >> choice))
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout << "Invalid input.\n";
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        if (choice == 4)
            break;

        if (choice < 1 || choice > 3)
        {
            std::cout
                << "Invalid choice. Try again.\n";

            continue;
        }

        std::vector<int> referenceString;
        int totalFrames = 0;

        if (!readSimulationInput(
                referenceString,
                totalFrames
            ))
        {
            continue;
        }

        try
        {
            if (choice == 1)
            {
                std::cout << "\nSelect Algorithm:\n";
                std::cout << "1. FIFO\n";
                std::cout << "2. LRU\n";
                std::cout << "3. Optimal\n";

                std::cout << "Enter algorithm choice: ";

                int algorithmChoice;

                if (!(std::cin >> algorithmChoice))
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout
                        << "Invalid algorithm choice.\n";

                    continue;
                }

                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                    '\n'
                );

                if (algorithmChoice < 1 ||
                    algorithmChoice > 3)
                {
                    std::cout
                        << "Invalid algorithm choice.\n";

                    continue;
                }

                runSingleAlgorithm(
                    referenceString,
                    totalFrames,
                    algorithmChoice
                );
            }
            else if (choice == 2)
            {
                runAllComparisons(
                    referenceString,
                    totalFrames
                );
            }
            else if (choice == 3)
            {
                checkBeladysAnomaly(
                    referenceString,
                    totalFrames
                );
            }
        }
        catch (const std::exception& error)
        {
            std::cout
                << "Simulation error: "
                << error.what()
                << '\n';
        }

    } while (true);
}
