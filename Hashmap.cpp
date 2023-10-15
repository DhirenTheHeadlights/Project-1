#include "Hashmap.h"

Hashmap::Hashmap(Map& map, sf::RenderWindow& window) {
}

void Hashmap::assignPellet(Pellet& pellet, Map& map) { // Assigns a pellet to a cell in the hashmap
    std::string key = generateKey(
        static_cast<int>(pellet.getPosition().x / map.getCellSize()),
        static_cast<int>(pellet.getPosition().y / map.getCellSize()));
    if (pellet.isActive()) {
        hashmap[key].insert(&pellet);
    }
    else {
        if (hashmap.count(key)) {
            hashmap[key].erase(&pellet);
        }
    }
}

std::string Hashmap::generateKey(int x, int y) const { // Generates a key for the hashmap
    return std::to_string(x) + "," + std::to_string(y);
}

std::vector<Pellet*> Hashmap::getPelletsInSameCell(float x, float y, Map& map) const { // Returns a vector of pellets in the same cell
    std::string key = generateKey(
        static_cast<int>(x / map.getCellSize()),
        static_cast<int>(y / map.getCellSize()));
    if (hashmap.count(key)) {
        const std::set<Pellet*>& pelletSet = hashmap.at(key);
        return std::vector<Pellet*>(pelletSet.begin(), pelletSet.end());
    }
    else {
        return std::vector<Pellet*>(); // return an empty vector if no key found
    }
}


std::set<Pellet*> Hashmap::checkCollision(Circle& circle, Map& map) {
    std::set<Pellet*> collidedPellets;

    // Get the bounding box of the circle
    float left = circle.getPosition().x;
    float right = circle.getPosition().x + 2 * circle.getCircleSize();
    float top = circle.getPosition().y;
    float bottom = circle.getPosition().y + 2 * circle.getCircleSize();

    // Convert the bounding box corners to grid coordinates
    auto topLeft = map.getGridCoordinates(left, top);
    auto bottomRight = map.getGridCoordinates(right, bottom);

    // Loop through all cells covered by the circle bounding box
    for (int i = topLeft.first; i <= bottomRight.first; i++) {
        for (int j = topLeft.second; j <= bottomRight.second; j++) {
            std::string key = generateKey(i, j);
            if (hashmap.find(key) != hashmap.end()) {
                for (Pellet* pellet : hashmap[key]) {
                    float circleCenterX = circle.getPosition().x + circle.getCircleSize();
                    float circleCenterY = circle.getPosition().y + circle.getCircleSize();
                    float dx = pellet->getPosition().x - circleCenterX;
                    float dy = pellet->getPosition().y - circleCenterY;
                    float distance = std::sqrt(dx * dx + dy * dy);

                    // Check for collision
                    if (distance < pellet->getRadius() + circle.getCircleSize()) {
                        collidedPellets.insert(pellet);
                    }
                }
            }
        }
    }

    return collidedPellets;
}