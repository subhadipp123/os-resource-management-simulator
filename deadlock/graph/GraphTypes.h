
#ifndef GRAPH_TYPES_H
#define GRAPH_TYPES_H

#include <string>

enum class EdgeType
{
    Request,
    Allocation
};

struct ProcessNode
{
    int id;
    std::string name;
};

struct ResourceNode
{
    int id;
    std::string name;

    int totalInstances;
    int availableInstances;
};

struct GraphEdge
{
    int processId;
    int resourceId;

    EdgeType type;

    // Number of resource instances requested or allocated.
    int units;
};

#endif
