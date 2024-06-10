#include "AStarAlgorithm_PG.h"

using namespace PirateGame;

void AStar::setStartAndEndPoints(const sf::Vector2f& startPos, const sf::Vector2f& endPos) {
    start = vectorToGrid(startPos);
    end = vectorToGrid(endPos);
    cachedPath = findPath();
}

void AStar::update(const sf::Vector2f& currentPosition) {
    // Re-calculate path every 2000 pixels away from the start
    if (vm::distance(currentPosition, gridToVector(start)) > 2000.f) {
        start = vectorToGrid(currentPosition);
        cachedPath = findPath();
    }

    if (cachedPath.empty()) {
        return;
    }

    sf::Vector2i currentGridPos = vectorToGrid(currentPosition);
    size_t nearestPointIndex = getNearestPathIndex(currentGridPos);

    if (nearestPointIndex < cachedPath.size() - 1) {
        sf::Vector2i nextGridPos = cachedPath[nearestPointIndex + 1];

        if (hasObstacleChanged(nextGridPos)) {
            auto subPath = findPath();
            cachedPath.erase(cachedPath.begin() + nearestPointIndex + 1, cachedPath.end());
            cachedPath.insert(cachedPath.end(), subPath.begin(), subPath.end());
        }
    }
}

sf::Vector2f AStar::getNextPoint(const sf::Vector2f& currentPosition) {
    if (cachedPath.empty()) {
        return gridToVector(end);
    }

    sf::Vector2i currentGridPos = vectorToGrid(currentPosition);
    size_t nearestPointIndex = getNearestPathIndex(currentGridPos);

    if (nearestPointIndex < cachedPath.size() - 1) {
        return gridToVector(cachedPath[nearestPointIndex + 1]);
    }
    else {
        return gridToVector(cachedPath.back()); // or some default value
    }
}

size_t AStar::getNearestPathIndex(const sf::Vector2i& currentPos) const {
    size_t nearestIndex = 0;
    float nearestDistance = heuristic(currentPos, cachedPath[0]);

    for (size_t i = 1; i < cachedPath.size(); ++i) {
        float distance = heuristic(currentPos, cachedPath[i]);
        if (distance < nearestDistance) {
            nearestIndex = i;
            nearestDistance = distance;
        }
    }

    return nearestIndex;
}

bool AStar::hasObstacleChanged(const sf::Vector2i& gridPos) const {
    for (const auto& sprite : nearbySprites) {
        sf::FloatRect bounds = sprite.getGlobalBounds();
        if (bounds.contains(gridToVector(gridPos))) {
            return true;
        }
    }
    return false;
}

std::vector<sf::Vector2i> AStar::findPath() {
    std::priority_queue<std::shared_ptr<AstarNode>, std::vector<std::shared_ptr<AstarNode>>, NodeComparator> openSet;
    std::unordered_map<sf::Vector2i, std::shared_ptr<AstarNode>, std::hash<sf::Vector2i>, std::equal_to<sf::Vector2i>> allNodes;
    std::unordered_set<sf::Vector2i, std::hash<sf::Vector2i>, std::equal_to<sf::Vector2i>> closedSet;

    auto startNode = std::make_shared<AstarNode>(start, 0.0f, heuristic(start, end));
    openSet.emplace(startNode);
    allNodes[start] = startNode;

    int iterations = 0;
    const int maxIterations = 10000; // Set an appropriate limit for debugging

    while (!openSet.empty() && iterations < maxIterations) {
        auto current = openSet.top();
        openSet.pop();

        if (current->position == end) {
            return reconstructPath(current);
        }

        closedSet.insert(current->position);

        for (const auto& neighborPos : getNeighbors(current->position)) {
            if (closedSet.count(neighborPos)) {
                continue;
            }

            if (isObstacle(neighborPos)) {
                continue;
            }

            float tentativeGCost = current->gCost + heuristic(current->position, neighborPos);
            if (allNodes.find(neighborPos) == allNodes.end() || tentativeGCost < allNodes[neighborPos]->gCost) {
                auto neighbor = std::make_shared<AstarNode>(neighborPos, tentativeGCost, heuristic(neighborPos, end), current);
                openSet.emplace(neighbor);
                allNodes[neighborPos] = neighbor;
            }
        }

        ++iterations;
    }

    return {};
}


std::vector<sf::Vector2i> AStar::getNeighbors(const sf::Vector2i& node) {
    return {
        {node.x + 1, node.y}, {node.x - 1, node.y},
        {node.x, node.y + 1}, {node.x, node.y - 1},
        {node.x + 1, node.y + 1}, {node.x - 1, node.y - 1},
        {node.x + 1, node.y - 1}, {node.x - 1, node.y + 1}
    };
}

float AStar::heuristic(const sf::Vector2i& a, const sf::Vector2i& b) const {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

std::vector<sf::Vector2i> AStar::reconstructPath(std::shared_ptr<AstarNode> node) const {
    std::vector<sf::Vector2i> path;
    while (node != nullptr) {
        path.push_back(node->position);
        node = node->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

sf::Vector2i AStar::vectorToGrid(const sf::Vector2f& vector) const {
    return sf::Vector2i(std::floor(vector.x / tileSize), std::floor(vector.y / tileSize));
}

sf::Vector2f AStar::gridToVector(const sf::Vector2i& grid) const {
    return sf::Vector2f(static_cast<int>(grid.x) * tileSize, static_cast<int>(grid.y) * tileSize);
}

bool AStar::isObstacle(const sf::Vector2i& node) const {
    for (const auto& sprite : nearbySprites) {
        sf::FloatRect bounds = sprite.getGlobalBounds();
        sf::FloatRect nodeBounds(node.x * tileSize, node.y * tileSize, tileSize, tileSize);
        if (bounds.intersects(nodeBounds)) {
            return true;
        }
    }
    return false;
}

void AStar::drawDebug(sf::RenderWindow* window) {
    for (const auto& pos : cachedPath) {
        sf::RectangleShape rectangle(sf::Vector2f(tileSize, tileSize));
        rectangle.setPosition(gridToVector(pos));
        rectangle.setFillColor(sf::Color::Green);
        window->draw(rectangle);
    }

    for (const auto& sprite : nearbySprites) {
        sf::FloatRect bounds = sprite.getGlobalBounds();
        sf::Vector2i topLeft = vectorToGrid(sf::Vector2f(bounds.left, bounds.top));
        sf::Vector2i bottomRight = vectorToGrid(sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height));

        for (int x = topLeft.x; x <= bottomRight.x; ++x) {
            for (int y = topLeft.y; y <= bottomRight.y; ++y) {
                sf::RectangleShape rectangle(sf::Vector2f(tileSize, tileSize));
                rectangle.setPosition(gridToVector(sf::Vector2i(x, y)));
                rectangle.setFillColor(sf::Color::Red);
                window->draw(rectangle);
            }
        }
    }

    for (int x = 0; x < window->getSize().x / tileSize; ++x) {
        for (int y = 0; y < window->getSize().y / tileSize; ++y) {
            sf::Vector2i pos(x, y);
            if (std::find(cachedPath.begin(), cachedPath.end(), pos) == cachedPath.end() &&
                !isObstacle(pos)) {
                sf::RectangleShape rectangle(sf::Vector2f(tileSize, tileSize));
                rectangle.setPosition(gridToVector(pos));
                rectangle.setFillColor(sf::Color::Yellow);
                window->draw(rectangle);
            }
        }
    }
}