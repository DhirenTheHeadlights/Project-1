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

    for (int i = topLeft.first; i <= bottomRight.first; ++i) {
        for (int j = topLeft.second; j <= bottomRight.second; ++j) {
            std::pair<int, int> key = generateKey(sf::Vector2f(static_cast<float>(i), static_cast<float>(j)));
            this->hashmap[key] = ship;
        }
    }
}

// Remove an object from the hashmap
void ShipHashmap::removeEnemyShip(EnemyShip* ship) {
    auto topLeft = map.getGridCoordinates(ship->getSprite().getPosition().x, ship->getSprite().getPosition().y);
    auto bottomRight = map.getGridCoordinates(ship->getSprite().getPosition().x + ship->getSprite().getGlobalBounds().width,
        ship->getSprite().getPosition().y + ship->getSprite().getGlobalBounds().height);

    for (int i = topLeft.first; i <= bottomRight.first; ++i) {
        for (int j = topLeft.second; j <= bottomRight.second; ++j) {
            std::pair<int, int> key = generateKey(sf::Vector2f(static_cast<float>(i), static_cast<float>(j)));
            this->hashmap.erase(key);
        }
    }
}

// Find ship near to a player, debug is used to visualize the grid cells being checked
std::set<EnemyShip*> ShipHashmap::findEnemyShipsNearShip(Ship* ship, float maxDistance, bool debug) {
    // Grab window
    sf::RenderWindow* window = GlobalValues::getInstance().getWindow();

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


