#include "CollisionHandler.h"

CollisionHandler::CollisionHandler(sf::RenderWindow& window, Map& map) : window(window), hashmap(window) {
}

void CollisionHandler::assignPelletToHM(std::vector<std::shared_ptr<Pellet>> activePellets, Map& map) {
    for (const auto& pelletPtr : activePellets) {
		hashmap.assignPellet(*pelletPtr, map);
	}
}

void CollisionHandler::assignCellToHM(Circle& cell, Map& map) {
	hashmap.assignCell(cell, map);
}

std::set<Pellet*> CollisionHandler::pelletCollisionWithinBoundsOfCircle(Circle* circle, Map& map, Debug& debug) {
    std::set<Pellet*> collidedPellets;
    float left = circle->getPosition().x - circle->getCircleSize();
    float right = circle->getPosition().x + circle->getCircleSize();
    float top = circle->getPosition().y - circle->getCircleSize();
    float bottom = circle->getPosition().y + circle->getCircleSize();
    auto topLeft = map.getGridCoordinates(left, top);
    auto bottomRight = map.getGridCoordinates(right, bottom);
    for (int i = topLeft.first; i <= bottomRight.first; i++) {
        for (int j = topLeft.second; j <= bottomRight.second; j++) {
            std::string key = hashmap.generateKey(i, j);
            checkCollisionWithPellet(key, circle, collidedPellets);
            debug.drawCell(i, j, map, sf::Color::White);
        }
    }
    return collidedPellets;
}

void CollisionHandler::removePelletWhenCollision(Circle* cell, Map& map, Debug& debug) {
    std::set<Pellet*> collidedPellets = pelletCollisionWithinBoundsOfCircle(cell, map, debug);
    std::set<Pellet*> toBeRemoved;
    float numActiveCollisions = 0;  // to keep track of number of active collisions this frame
    for (Pellet* collidedPelletPtr : collidedPellets) {
        if (collidedPelletPtr->getRadius() > cell->getCircleSize()) {
            continue;
        }
        if (!collidedPelletPtr->isActive()) {
            continue; // Skip pellets that have already been deactivated in a previous collision
        }
        collidedPelletPtr->deActivate();
        numActiveCollisions += static_cast<float>(0.1 * collidedPelletPtr->getRadius());
    }
    if (numActiveCollisions > 0) {
        cell->setCircleSize(cell->getCircleSize() + numActiveCollisions);
    }
}

void CollisionHandler::checkCollisionWithPellet(const std::string& key, Circle* circle, std::set<Pellet*>& collidedPellets) {
    auto& mapReference = hashmap.getHashmapUnorderedMap();
    if (mapReference.find(key) != mapReference.end()) {
        for (Pellet* pellet : mapReference[key]) {
            float dx = pellet->getPosition().x - circle->getPosition().x;
            float dy = pellet->getPosition().y - circle->getPosition().y;
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance < (pellet->getRadius() * minDistForCollision + circle->getCircleSize())) collidedPellets.insert(pellet);
        }
    }
}

void CollisionHandler::checkCollisionForEveryCell(Map& map, CellGroup& cells, Debug& debug) {
    for (Circle* cell : cells.getCellGroup()) removePelletWhenCollision(cell, map, debug);
}

void CollisionHandler::separateCells(Circle* cell, Map& map) {
    // Identify the cells that are in close proximity to the current cell
    std::set<Circle*> nearbyCells = hashmap.getCellsInSameCell(cell, map);
    for (Circle* nearbyCell : nearbyCells) {
        float dx = nearbyCell->getPosition().x - cell->getPosition().x;
        float dy = nearbyCell->getPosition().y - cell->getPosition().y;
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < nearbyCell->getCircleSize() + cell->getCircleSize()) {
            float adjustMultiplier = 15.f;
            float overlap = (nearbyCell->getCircleSize() + cell->getCircleSize()) - distance;
            float adjustX = ((dx / distance) * overlap) + 0.5f;
            float adjustY = ((dy / distance) * overlap) + 0.5f;
            nearbyCell->setPosition(nearbyCell->getPosition().x + adjustX * adjustMultiplier, nearbyCell->getPosition().y + adjustY * adjustMultiplier);
            cell->setPosition(cell->getPosition().x - adjustX * adjustMultiplier, cell->getPosition().y - adjustY * adjustMultiplier);
            std::cout << "overlap: " << overlap << std::endl;
            nearbyCell->setColor(sf::Color::Red);
            cell->setColor(sf::Color::Red);
        }
        hashmap.assignCell(*nearbyCell, map);
        hashmap.assignCell(*cell, map);
    }
}