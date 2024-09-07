#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <memory>
#include <algorithm>
#include <iostream>

#include "VectorMath.h"

namespace std {
    template <>
    struct hash<sf::Vector2i> {
        std::size_t operator()(const sf::Vector2i& v) const noexcept {
            return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
        }
    };

    template <>
    struct equal_to<sf::Vector2i> {
        bool operator()(const sf::Vector2i& lhs, const sf::Vector2i& rhs) const noexcept {
            return lhs.x == rhs.x && lhs.y == rhs.y;
        }
    };
}

namespace PirateGame {
    const int tileSize = 256;
    const float boundPadding = 1.f;

    enum class NodeType {
        Path,
        Obstacle
    };

    struct AstarNode {
        sf::Vector2i position;
        float gCost;
        float hCost;
        std::shared_ptr<AstarNode> parent;

        AstarNode(sf::Vector2i pos, float g, float h, std::shared_ptr<AstarNode> p = nullptr)
            : position(pos), gCost(g), hCost(h), parent(p) {}

        float fCost() const noexcept { return gCost + hCost; }
        bool operator==(const AstarNode& other) const noexcept { return position == other.position; }
        sf::FloatRect getRect() const noexcept { return sf::FloatRect(position.x * static_cast<float>(tileSize), position.y * static_cast<float>(tileSize), static_cast<float>(tileSize), static_cast<float>(tileSize)); }
    };

    struct NodeComparator {
        bool operator()(const std::shared_ptr<AstarNode>& lhs, const std::shared_ptr<AstarNode>& rhs) const noexcept {
            return lhs->fCost() > rhs->fCost();
        }
    };

    class AStar {
    public:
        AStar() {}

        void setStartAndEndPoints(const sf::Vector2f& startPos, const sf::Vector2f& endPos);

        void update(const sf::Vector2f& currentPosition);

        void updateNearbySprites(const std::vector<sf::Sprite>& newNearbySprites) { nearbySprites = newNearbySprites; }

        sf::Vector2f getNextPoint(const sf::Vector2f& currentPosition) const;

        void drawDebug(sf::RenderWindow* window);

        void recalculatePath() { cachedPath = findPath(); }

    private:
        sf::Vector2i start;
        sf::Vector2i end;

        std::size_t lastSpritesHash = 0;
        const int maxIterations = 10000;
        const float reCalculatePathInterval = 2000.f;

        sf::Clock pathRecalculationClock;
        sf::Time pathRecalculationCooldown = sf::seconds(2.f);

        std::vector<sf::Sprite> nearbySprites;

        std::vector<sf::Vector2i> cachedPath;

        size_t getNearestPathIndex(const sf::Vector2i& currentPos) const;

        bool hasObstacleChanged(const sf::Vector2i& gridPos) const;

        std::vector<sf::Vector2i> findPath();

        static std::vector<sf::Vector2i> getNeighbors(const sf::Vector2i& node) noexcept;

        float heuristic(const sf::Vector2i& a, const sf::Vector2i& b) const noexcept;

        std::vector<sf::Vector2i> reconstructPath(std::shared_ptr<AstarNode> node) const;

        sf::Vector2i vectorToGrid(const sf::Vector2f& vector) const noexcept;

        sf::Vector2f gridToVector(const sf::Vector2i& grid) const noexcept;

        bool isObstacle(const sf::Vector2i& node) const;

        std::priority_queue<std::shared_ptr<AstarNode>, std::vector<std::shared_ptr<AstarNode>>, NodeComparator> openSet;
    };
}
