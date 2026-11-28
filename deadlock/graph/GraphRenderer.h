
#ifndef GRAPH_RENDERER_H
#define GRAPH_RENDERER_H

#include "ResourceAllocationGraph.h"
#include "../detection/DeadlockDetector.h"

#include <SFML/Graphics.hpp>
#include <string>

class GraphRenderer
{
private:
    sf::RenderWindow window;
    sf::Font font;

    const ResourceAllocationGraph& graph;

    DetectionResult detectionResult;

    bool isDeadlockedProcess(int processId) const;

    void drawProcess(
        float x,
        float y,
        const std::string& label,
        bool deadlocked
    );

    void drawResource(
        float x,
        float y,
        const std::string& label,
        int totalInstances,
        int availableInstances
    );

    void drawArrow(
        sf::Vector2f start,
        sf::Vector2f end,
        sf::Color color
    );

    void drawEdgeLabel(
        sf::Vector2f start,
        sf::Vector2f end,
        const std::string& label,
        sf::Color color
    );

    void drawInfoPanel();

    void drawLegend();

    void drawText(
        const std::string& content,
        unsigned int size,
        sf::Color color,
        float x,
        float y
    );

public:
    explicit GraphRenderer(
        const ResourceAllocationGraph& graph
    );

    void run();
};

#endif
