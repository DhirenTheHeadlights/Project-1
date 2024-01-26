#include "CollisionManager_AG.h"

CollisionManager::CollisionManager(sf::RenderWindow& window, Map& map) : hashmap(window), debugger(window) {
}

void CollisionManager::moveAndSeparateCells(CellGroup& cells, Map& map, sf::RenderWindow& window, float moveSpeed) { // Moves and separates all cells
    cells.move(moveSpeed, map, window);
	for (int i = 0; i < 5; i++) {
        separateAllCells(cells, map);
    }
	//std::cout << "Cells moved and separated" << std::endl; // For debugging
}

void CollisionManager::assignCells(CellGroup& cells, Map& map) {
    for (Circle* cell : cells.getCellGroup()) {
		hashmap.assignCell(*cell, map);
	}
    //std::cout << "Cells assigned" << std::endl;  // for debug
}
void CollisionManager::assignPellets(std::vector<std::shared_ptr<Pellet>> activePellets, Map& map) { // Assigns all pellets to the hashmap
    for (std::shared_ptr<Pellet> pelletPtr : activePellets) {
		hashmap.assignPellet(*pelletPtr, map);
	}
	//std::cout << "Pellets assigned" << std::endl; // For debugging
}

std::set<Pellet*> CollisionManager::pelletCollisionWithinBoundsOfCircle(sf::RenderWindow& window, Circle* circle, Map& map) { // Returns a set of pointers to pellets that are colliding with the circle
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
            debugger.drawCell(window, i, j, map, sf::Color::White);
        }
    }
    //std::cout << "Collided pellets: " << collidedPellets.size() << std::endl; // For debugging
    return collidedPellets;
}

void CollisionManager::checkCollisionWithPellet(const std::string& key, Circle* circle, std::set<Pellet*>& collidedPellets) { // Checks if there is a collision with a pellet in the hashmap
    auto it = hashmap.getPelletHashmap().find(key);
    if (it != hashmap.getPelletHashmap().end()) {
        for (Pellet* pellet : it->second) {
            float dx = pellet->getPosition().x - circle->getPosition().x;
            float dy = pellet->getPosition().y - circle->getPosition().y;
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance < (pellet->getRadius() * minDistForCollision + circle->getCircleSize())) collidedPellets.insert(pellet);
        }
        //std::cout << "Collision with Pellet checked.!!! (men kissed) \n"; // For debugging
    }
}

void CollisionManager::checkPelletCollisionForEveryCell(sf::RenderWindow& window, CellGroup& cells, Map& map, std::vector<std::shared_ptr<Pellet>> activePellets) { // Checks for collisions between every cell and every pellet
    for (Circle* cell : cells.getCellGroup()) removePelletWhenCollision(window, cell, map, activePellets);
    //std::cout << "Pellet collisions checked" << std::endl; // For debugging 
}

void CollisionManager::separateAllCells(CellGroup& cells, Map& map) { // Separates all cells from each other
	for (Circle* cell : cells.getCellGroup()) separateCells(cell, map);
	//std::cout << "Cells separated" << std::endl; // For debugging
}

void CollisionManager::separateCells(Circle* cell, Map& map) { // Separates a single cell from other cells
    // Identify the cells that are in close proximity to the current cell
    std::set<Circle*> nearbyCells = hashmap.getCellsInSameCell(cell, map);
    for (Circle* nearbyCell : nearbyCells) {
        float dx = nearbyCell->getPosition().x - cell->getPosition().x;
        float dy = nearbyCell->getPosition().y - cell->getPosition().y;
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < nearbyCell->getCircleSize() + cell->getCircleSize()) {
            float adjustMultiplier = 0.5f;
            float overlap = (nearbyCell->getCircleSize() + cell->getCircleSize()) - distance;
            float adjustX = ((dx / distance) * overlap) + 0.5f;
            float adjustY = ((dy / distance) * overlap) + 0.5f;
            nearbyCell->setPosition(nearbyCell->getPosition().x + adjustX * adjustMultiplier, nearbyCell->getPosition().y + adjustY * adjustMultiplier);
            cell->setPosition(cell->getPosition().x - adjustX * adjustMultiplier, cell->getPosition().y - adjustY * adjustMultiplier);
            //std::cout << "overlap: " << overlap << std::endl;
            nearbyCell->setColor(sf::Color::Red);
            cell->setColor(sf::Color::Red);
        }
    }
    //std::cout << "Cells separated" << std::endl; // For debugging
}

void CollisionManager::removePelletWhenCollision(sf::RenderWindow& window, Circle* cell, Map& map, std::vector<std::shared_ptr<Pellet>> activePellets) { // Removes pellets from the activePellets vector when they collide with a cell
    std::set<Pellet*> collidedPellets = pelletCollisionWithinBoundsOfCircle(window, cell, map);
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
    //std::cout << "Pellets removed" << std::endl; // For debugging
}


