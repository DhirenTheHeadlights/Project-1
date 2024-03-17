#include "ShipHashmap_PG.h"

using namespace PirateGame;

// Generate a key for the hashmap based on position
std::pair<int, int> ShipHashmap::generateKey(sf::Vector2f pos) {
    return { pos.x, pos.y };
}

// Add a new object to the hashmap
void ShipHashmap::addEnemyShip(EnemyShip* ship) {
    // Use the bounding box of the sprite for grid calculations
    sf::FloatRect bounds = ship->getSprite().getGlobalBounds();
    auto topLeft = map.getGridCoordinates(bounds.left, bounds.top);
    auto bottomRight = map.getGridCoordinates(bounds.left + bounds.width, bounds.top + bounds.height);

    std::set<std::pair<int, int>> occupiedPositions;

    // Add the ship to the primary hashmap
    for (int i = topLeft.first; i <= bottomRight.first; ++i) {
        for (int j = topLeft.second; j <= bottomRight.second; ++j) {
            std::pair<int, int> key = { i, j };
            this->hashmap[key] = ship; // Update the primary hashmap
            occupiedPositions.insert(key); // Store positions for the reverse mapping
        }
    }

    // Add the ship to the reverse hashmap
    this->reverseHashmap[ship] = occupiedPositions;
}

// Remove an object from the hashmap
void ShipHashmap::removeEnemyShip(EnemyShip* ship) {
    // Fetch the ship's current positions from the reverse mapping
    if (this->reverseHashmap.find(ship) != this->reverseHashmap.end()) {
        const std::set<std::pair<int, int>>& occupiedPositions = this->reverseHashmap[ship];

        for (const auto& pos : occupiedPositions) {
            this->hashmap.erase(pos); // Remove the ship from these positions in the primary hashmap
        }

        // Remove the ship's entry from the reverse mapping
        this->reverseHashmap.erase(ship);
    }
}

// Find ship near to a player, debug is used to visualize the grid cells being checked
std::set<EnemyShip*> ShipHashmap::findEnemyShipsNearShip(Ship* ship, float maxDistance, bool debug) {
    // Grab window
    sf::RenderWindow* window = GlobalValues::getInstance().getWindow();

    maxDistance /= 2; // Divide by 2 to get the radius

    // Get the position of the ship
    sf::Vector2f shipPosition = ship->getSprite().getPosition();

    // Calculate the extended area around the ship based on maxDistance
    float left = shipPosition.x - maxDistance;
    float right = shipPosition.x + maxDistance;
    float top = shipPosition.y - maxDistance;
    float bottom = shipPosition.y + maxDistance;

    // Get the top left and bottom right cells of the bounding box
    auto topLeft = map.getGridCoordinates(left, top);
    auto bottomRight = map.getGridCoordinates(right, bottom);

    std::set<EnemyShip*> nearbyShips;

    // Check the cells in the bounding box
    for (int i = topLeft.first; i <= bottomRight.first; i++) {
        for (int j = topLeft.second; j <= bottomRight.second; j++) {
            std::pair<int, int> key = generateKey(sf::Vector2f(static_cast<float>(i), static_cast<float>(j)));

            // If the key exists in the hashmap and is within maxDistance, add the ship to the set
            if (hashmap.count(key)) {
                nearbyShips.insert(hashmap.at(key));
            }

            if (!debug) {
                continue;
            }

            // Optionally, visualize the grid cells being checked (for debugging)
            sf::RectangleShape rect;
            rect.setSize(sf::Vector2f(static_cast<float>(map.getCellSize()), static_cast<float>(map.getCellSize())));
            rect.setFillColor(sf::Color::Magenta);  // Color for visualization
            rect.setPosition(sf::Vector2f(i * static_cast<float>(map.getCellSize()), j * static_cast<float>(map.getCellSize())));
            window->draw(rect);
        }
    }

    return nearbyShips;
}

// Update the ship's position in the hashmap
void ShipHashmap::updateEnemyShipPosition(EnemyShip* ship) {
    // Get the ship's new position from its sprite and calculate new grid cells it occupies
    sf::FloatRect bounds = ship->getSprite().getGlobalBounds();
    auto newTopLeft = map.getGridCoordinates(bounds.left, bounds.top);
    auto newBottomRight = map.getGridCoordinates(bounds.left + bounds.width, bounds.top + bounds.height);

    std::set<std::pair<int, int>> newPositions;
    for (int i = newTopLeft.first; i <= newBottomRight.first; ++i) {
        for (int j = newTopLeft.second; j <= newBottomRight.second; ++j) {
            newPositions.insert({ i, j });
        }
    }

    // Retrieve the ship's current positions from the reverse hashmap
    const auto& currentPositions = reverseHashmap[ship];

    std::set<std::pair<int, int>> positionsToAdd;
    std::set<std::pair<int, int>> positionsToRemove;

    // Determine which new positions need to be added (those not already in currentPositions)
    for (const auto& newPos : newPositions) {
        if (currentPositions.find(newPos) == currentPositions.end()) {
            positionsToAdd.insert(newPos);
        }
    }

    // Determine which positions need to be removed (those not in newPositions)
    for (const auto& pos : currentPositions) {
        if (newPositions.find(pos) == newPositions.end()) {
            positionsToRemove.insert(pos);
        }
    }

    // Update the hashmap and reverseHashmap
    for (const auto& pos : positionsToRemove) {
        hashmap.erase(pos); // Remove the ship from old positions in the primary hashmap
    }
    for (const auto& newPos : positionsToAdd) {
        hashmap[newPos] = ship; // Add or update the ship in new positions in the primary hashmap
        reverseHashmap[ship].insert(newPos); // Update the reverse hashmap
    }

    // Update the ship's current positions in the reverseHashmap
    // This could be optimized by directly modifying the set, but for clarity, we're simply replacing it here
    reverseHashmap[ship] = newPositions;
}
