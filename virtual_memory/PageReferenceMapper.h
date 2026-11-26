
#ifndef PAGE_REFERENCE_MAPPER_H
#define PAGE_REFERENCE_MAPPER_H

#include <limits>
#include <stdexcept>
#include <unordered_map>
#include <vector>

struct PageReferenceMapping
{
    std::vector<int> compactReferences;
    std::vector<int> originalPages;
};

inline PageReferenceMapping mapPageReferences(
    const std::vector<int>& referenceString
)
{
    if (referenceString.empty())
    {
        throw std::invalid_argument(
            "Reference string cannot be empty."
        );
    }

    PageReferenceMapping mapping;

    std::unordered_map<int, int> pageToId;

    mapping.compactReferences.reserve(referenceString.size());

    for (int page : referenceString)
    {
        if (page < 0)
        {
            throw std::invalid_argument(
                "Page numbers cannot be negative."
            );
        }

        auto it = pageToId.find(page);

        if (it != pageToId.end())
        {
            mapping.compactReferences.push_back(it->second);
            continue;
        }

        if (mapping.originalPages.size() >=
            static_cast<std::size_t>(
                std::numeric_limits<int>::max()
            ))
        {
            throw std::length_error(
                "Too many unique pages in reference string."
            );
        }

        int compactId =
            static_cast<int>(mapping.originalPages.size());

        pageToId.emplace(page, compactId);

        mapping.originalPages.push_back(page);
        mapping.compactReferences.push_back(compactId);
    }

    return mapping;
}

#endif
