
#include "GraphRenderer.h"

#include <iostream>
#include <cmath>
#include <algorithm>
#include <optional>

GraphRenderer::GraphRenderer(
    const ResourceAllocationGraph& graph
)
    : window(
        sf::VideoMode({1200, 800}),
        "OS Resource Management Simulator - Deadlock Graph"
    ),
      graph(graph),
      detectionResult(
          DeadlockDetector::detectMultipleInstanceDeadlock(graph)
      )
{
    window.setFramerateLimit(60);

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Failed to load font!\n";
    }
}

bool GraphRenderer::isDeadlockedProcess(int processId) const
{
    return std::find(
        detectionResult.deadlockedProcessIds.begin(),
        detectionResult.deadlockedProcessIds.end(),
        processId
    ) != detectionResult.deadlockedProcessIds.end();
}

void GraphRenderer::drawText(
    const std::string& content,
    unsigned int size,
    sf::Color color,
    float x,
    float y)
{
    sf::Text text(font, content, size);

    text.setFillColor(color);
    text.setPosition({x, y});

    window.draw(text);
}

void GraphRenderer::drawProcess(
    float x,
    float y,
    const std::string& label,
    bool deadlocked)
{
    sf::CircleShape process(40.f);

    if (deadlocked)
    {
        process.setFillColor(sf::Color(190, 45, 55));
        process.setOutlineColor(sf::Color(255, 100, 100));
    }
    else
    {
        process.setFillColor(sf::Color(50, 120, 220));
        process.setOutlineColor(sf::Color::White);
    }

    process.setOutlineThickness(3.f);
    process.setPosition({x - 40.f, y - 40.f});

    window.draw(process);

    sf::Text text(font, label, 22);

    text.setFillColor(sf::Color::White);

    sf::FloatRect bounds = text.getLocalBounds();

    text.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    });

    text.setPosition({x, y});

    window.draw(text);
}

void GraphRenderer::drawResource(
    float x,
    float y,
    const std::string& label,
    int totalInstances,
    int availableInstances)
{
    constexpr float width = 170.f;
    constexpr float height = 110.f;

    sf::RectangleShape resource({width, height});

    resource.setFillColor(sf::Color(40, 160, 100));
    resource.setOutlineThickness(3.f);
    resource.setOutlineColor(sf::Color::White);

    resource.setPosition({
        x - width / 2.f,
        y - height / 2.f
    });

    window.draw(resource);

    sf::Text name(font, label, 22);

    name.setFillColor(sf::Color::White);

    sf::FloatRect nameBounds = name.getLocalBounds();

    name.setOrigin({
        nameBounds.position.x + nameBounds.size.x / 2.f,
        nameBounds.position.y + nameBounds.size.y / 2.f
    });

    name.setPosition({x, y - 27.f});

    window.draw(name);

    sf::Text availability(
        font,
        "Free: " +
        std::to_string(availableInstances) +
        "/" +
        std::to_string(totalInstances),
        15
    );

    availability.setFillColor(sf::Color::White);

    sf::FloatRect availabilityBounds =
        availability.getLocalBounds();

    availability.setOrigin({
        availabilityBounds.position.x +
            availabilityBounds.size.x / 2.f,
        availabilityBounds.position.y +
            availabilityBounds.size.y / 2.f
    });

    availability.setPosition({x, y + 1.f});

    window.draw(availability);

    int visibleDots = std::min(totalInstances, 8);

    if (visibleDots <= 0)
        return;

    constexpr float spacing = 18.f;

    float totalWidth = (visibleDots - 1) * spacing;
    float startX = x - totalWidth / 2.f;

    int allocatedInstances =
        totalInstances - availableInstances;

    for (int i = 0; i < visibleDots; i++)
    {
        sf::CircleShape dot(5.f);

        if (i < allocatedInstances)
        {
            dot.setFillColor(sf::Color(255, 170, 40));
        }
        else
        {
            dot.setFillColor(sf::Color::White);
        }

        dot.setPosition({
            startX + i * spacing - 5.f,
            y + 27.f
        });

        window.draw(dot);
    }

    if (totalInstances > 8)
    {
        drawText(
            "+" + std::to_string(totalInstances - 8),
            13,
            sf::Color::White,
            x + 75.f,
            y + 18.f
        );
    }
}

void GraphRenderer::drawArrow(
    sf::Vector2f start,
    sf::Vector2f end,
    sf::Color color)
{
    float dx = end.x - start.x;
    float dy = end.y - start.y;

    float length = std::sqrt(dx * dx + dy * dy);

    if (length < 20.f)
        return;

    float ux = dx / length;
    float uy = dy / length;

    sf::Vector2f perpendicular(-uy, ux);

    sf::RectangleShape line({length - 14.f, 3.f});

    line.setFillColor(color);
    line.setOrigin({0.f, 1.5f});
    line.setPosition(start);

    float angle =
        std::atan2(dy, dx) * 180.f / 3.14159265f;

    line.setRotation(sf::degrees(angle));

    window.draw(line);

    sf::Vector2f base(
        end.x - ux * 14.f,
        end.y - uy * 14.f
    );

    sf::ConvexShape arrowHead(3);

    arrowHead.setPoint(0, end);

    arrowHead.setPoint(
        1,
        {
            base.x + perpendicular.x * 7.f,
            base.y + perpendicular.y * 7.f
        }
    );

    arrowHead.setPoint(
        2,
        {
            base.x - perpendicular.x * 7.f,
            base.y - perpendicular.y * 7.f
        }
    );

    arrowHead.setFillColor(color);

    window.draw(arrowHead);
}

void GraphRenderer::drawEdgeLabel(
    sf::Vector2f start,
    sf::Vector2f end,
    const std::string& label,
    sf::Color color)
{
    float dx = end.x - start.x;
    float dy = end.y - start.y;

    float length = std::sqrt(dx * dx + dy * dy);

    if (length < 1.f)
        return;

    constexpr float positionRatio = 0.35f;

    sf::Vector2f position(
        start.x + dx * positionRatio,
        start.y + dy * positionRatio
    );

    sf::Vector2f perpendicular(
        -dy / length,
        dx / length
    );

    float side =
        (color == sf::Color(255, 120, 120)) ? -1.f : 1.f;

    constexpr float offset = 16.f;

    sf::Vector2f labelPosition(
        position.x + perpendicular.x * offset * side,
        position.y + perpendicular.y * offset * side
    );

    sf::Text text(font, label, 14);

    text.setFillColor(color);

    sf::FloatRect bounds = text.getLocalBounds();

    text.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    });

    text.setPosition(labelPosition);

    sf::RectangleShape background({
        bounds.size.x + 12.f,
        bounds.size.y + 8.f
    });

    background.setOrigin({
        (bounds.size.x + 12.f) / 2.f,
        (bounds.size.y + 8.f) / 2.f
    });

    background.setPosition(labelPosition);
    background.setFillColor(sf::Color(25, 30, 40));

    window.draw(background);
    window.draw(text);
}

void GraphRenderer::drawInfoPanel()
{
    const auto& processes = graph.getProcesses();
    const auto& resources = graph.getResources();
    const auto& edges = graph.getEdges();

    int totalInstances = 0;
    int availableInstances = 0;

    for (const auto& resource : resources)
    {
        totalInstances += resource.totalInstances;
        availableInstances += resource.availableInstances;
    }

    sf::RectangleShape panel({510.f, 165.f});

    panel.setPosition({20.f, 15.f});
    panel.setFillColor(sf::Color(35, 43, 57));
    panel.setOutlineThickness(1.f);
    panel.setOutlineColor(sf::Color(90, 100, 120));

    window.draw(panel);

    drawText(
        "RESOURCE ALLOCATION GRAPH",
        19,
        sf::Color::White,
        38.f,
        28.f
    );

    drawText(
        "Processes: " + std::to_string(processes.size()),
        16,
        sf::Color(120, 180, 255),
        38.f,
        65.f
    );

    drawText(
        "Resources: " + std::to_string(resources.size()),
        16,
        sf::Color(100, 220, 150),
        195.f,
        65.f
    );

    drawText(
        "Edges: " + std::to_string(edges.size()),
        16,
        sf::Color(255, 190, 100),
        355.f,
        65.f
    );

    drawText(
        "Instances: " +
        std::to_string(availableInstances) +
        "/" +
        std::to_string(totalInstances) +
        " available",
        15,
        sf::Color(210, 215, 225),
        38.f,
        88.f
    );

    if (detectionResult.status == DetectionStatus::DeadlockDetected)
    {
        drawText(
            "DEADLOCK DETECTED!",
            20,
            sf::Color(255, 80, 80),
            38.f,
            115.f
        );

        std::string names = "Processes: ";

        for (int id : detectionResult.deadlockedProcessIds)
        {
            auto it = std::find_if(
                processes.begin(),
                processes.end(),
                [id](const ProcessNode& process)
                {
                    return process.id == id;
                }
            );

            if (it != processes.end())
                names += it->name + " ";
        }

        drawText(
            names,
            15,
            sf::Color(255, 160, 160),
            270.f,
            120.f
        );
    }
    else if (detectionResult.status == DetectionStatus::NoDeadlock)
    {
        drawText(
            "NO DEADLOCK DETECTED",
            20,
            sf::Color(70, 230, 130),
            38.f,
            120.f
        );
    }
    else if (detectionResult.status ==
             DetectionStatus::InvalidGraph)
    {
        drawText(
            "INVALID RESOURCE GRAPH",
            19,
            sf::Color(255, 190, 60),
            38.f,
            120.f
        );
    }
    else
    {
        drawText(
            "DETECTION UNSUPPORTED",
            19,
            sf::Color(255, 190, 60),
            38.f,
            120.f
        );
    }
}

void GraphRenderer::drawLegend()
{
    sf::RectangleShape panel({260.f, 145.f});

    panel.setPosition({920.f, 15.f});
    panel.setFillColor(sf::Color(35, 43, 57));
    panel.setOutlineThickness(1.f);
    panel.setOutlineColor(sf::Color(90, 100, 120));

    window.draw(panel);

    drawText(
        "LEGEND",
        17,
        sf::Color::White,
        940.f,
        25.f
    );

    sf::RectangleShape requestLine({25.f, 3.f});
    requestLine.setPosition({940.f, 57.f});
    requestLine.setFillColor(sf::Color(255, 70, 70));

    window.draw(requestLine);

    drawText(
        "Request",
        14,
        sf::Color::White,
        975.f,
        48.f
    );

    sf::RectangleShape allocationLine({25.f, 3.f});
    allocationLine.setPosition({940.f, 80.f});
    allocationLine.setFillColor(sf::Color(50, 220, 120));

    window.draw(allocationLine);

    drawText(
        "Allocation",
        14,
        sf::Color::White,
        975.f,
        71.f
    );

    sf::CircleShape allocatedDot(5.f);
    allocatedDot.setPosition({940.f, 96.f});
    allocatedDot.setFillColor(sf::Color(255, 170, 40));

    window.draw(allocatedDot);

    drawText(
        "Allocated",
        14,
        sf::Color::White,
        975.f,
        90.f
    );

    sf::CircleShape availableDot(5.f);
    availableDot.setPosition({1080.f, 96.f});
    availableDot.setFillColor(sf::Color::White);

    window.draw(availableDot);

    drawText(
        "Free",
        14,
        sf::Color::White,
        1095.f,
        90.f
    );

    sf::CircleShape deadlockDot(7.f);
    deadlockDot.setPosition({940.f, 120.f});
    deadlockDot.setFillColor(sf::Color(190, 45, 55));

    window.draw(deadlockDot);

    drawText(
        "Deadlocked Process",
        14,
        sf::Color::White,
        960.f,
        114.f
    );
}

void GraphRenderer::run()
{
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(25, 30, 40));

        const auto& processes = graph.getProcesses();
        const auto& resources = graph.getResources();
        const auto& edges = graph.getEdges();

        float width = static_cast<float>(window.getSize().x);
        float height = static_cast<float>(window.getSize().y);

        float processX = width * 0.20f;
        float resourceX = width * 0.75f;

        constexpr float topMargin = 220.f;
        constexpr float bottomMargin = 45.f;

        auto getY = [
            height,
            topMargin,
            bottomMargin
        ](
            std::size_t index,
            std::size_t count)
        {
            if (count == 0)
                return (topMargin + height - bottomMargin) / 2.f;

            float usableHeight =
                height - topMargin - bottomMargin;

            return topMargin +
                usableHeight *
                static_cast<float>(index + 1) /
                static_cast<float>(count + 1);
        };

        // Check whether a graph edge is identified
        // by the deadlock detector as a blocking relationship.
        auto isBlockingEdge = [&](const GraphEdge& edge)
        {
            auto matches = [&](const GraphEdgeReference& reference)
            {
                return reference.processId == edge.processId &&
                       reference.resourceId == edge.resourceId &&
                       reference.type == edge.type;
            };

            if (edge.type == EdgeType::Request)
            {
                return std::any_of(
                    detectionResult.blockedRequestEdges.begin(),
                    detectionResult.blockedRequestEdges.end(),
                    matches
                );
            }

            return std::any_of(
                detectionResult.blockingAllocationEdges.begin(),
                detectionResult.blockingAllocationEdges.end(),
                matches
            );
        };

        // Draw edges and quantity labels.
        for (const auto& edge : edges)
        {
            auto processIt = std::find_if(
                processes.begin(),
                processes.end(),
                [&](const ProcessNode& process)
                {
                    return process.id == edge.processId;
                }
            );

            auto resourceIt = std::find_if(
                resources.begin(),
                resources.end(),
                [&](const ResourceNode& resource)
                {
                    return resource.id == edge.resourceId;
                }
            );

            if (processIt == processes.end() ||
                resourceIt == resources.end())
            {
                continue;
            }

            std::size_t processIndex =
                static_cast<std::size_t>(
                    std::distance(processes.begin(), processIt)
                );

            std::size_t resourceIndex =
                static_cast<std::size_t>(
                    std::distance(resources.begin(), resourceIt)
                );

            float processY = getY(
                processIndex,
                processes.size()
            );

            float resourceY = getY(
                resourceIndex,
                resources.size()
            );

            bool blocking = isBlockingEdge(edge);

            sf::Color edgeColor;

            if (blocking)
            {
                edgeColor = sf::Color(255, 60, 60);
            }
            else if (edge.type == EdgeType::Request)
            {
                edgeColor = sf::Color(255, 70, 70);
            }
            else
            {
                edgeColor = sf::Color(50, 220, 120);
            }

            if (edge.type == EdgeType::Request)
            {
                sf::Vector2f start(
                    processX + 42.f,
                    processY
                );

                sf::Vector2f end(
                    resourceX - 87.f,
                    resourceY
                );

                drawArrow(start, end, edgeColor);

                drawEdgeLabel(
                    start,
                    end,
                    "Request: " + std::to_string(edge.units),
                    sf::Color(255, 120, 120)
                );
            }
            else
            {
                sf::Vector2f start(
                    resourceX - 87.f,
                    resourceY
                );

                sf::Vector2f end(
                    processX + 42.f,
                    processY
                );

                drawArrow(start, end, edgeColor);

                drawEdgeLabel(
                    start,
                    end,
                    "Allocation: " + std::to_string(edge.units),
                    sf::Color(100, 240, 150)
                );
            }
        }

        // Draw processes.
        for (std::size_t i = 0; i < processes.size(); i++)
        {
            drawProcess(
                processX,
                getY(i, processes.size()),
                processes[i].name,
                isDeadlockedProcess(processes[i].id)
            );
        }

        // Draw resources.
        for (std::size_t i = 0; i < resources.size(); i++)
        {
            drawResource(
                resourceX,
                getY(i, resources.size()),
                resources[i].name,
                resources[i].totalInstances,
                resources[i].availableInstances
            );
        }

        drawInfoPanel();
        drawLegend();

        window.display();
    }
}
