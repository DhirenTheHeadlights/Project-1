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


std::set<Pellet*> Hashm (Circle& circle, Map& map) { // Checks for collisions between the circle and pellets
    std::vector<Pellet*> nearbyPellets = getPelletsInSameCell(circle.getPosition().x, circle.getPosition().y, map);
    std::set<Pellet*> collidedPellets;        
    for (Pellet* pelletPtr : nearbyPellets) {
        Pellet& pellet = *pelletPtr;
        float dx = pellet.getPosition().x - circle.getPosition().x;
        float dy = pellet.getPosition().y - circle.getPosition().y;
        float distanceSquared = dx * dx + dy * dy;
        float sumRadii = circle.getCirclesize() + pellet.getRadius();
        if (distanceSquared <= sumRadii * sumRadii) {
            collidedPellets.insert(pelletPtr);
        }
    }
    return collidedPellets;
}
