#include "Hashmap.h"

Hashmap::Hashmap(Map& map, sf::RenderWindow& window) {
}

void Hashmap::assignPellet(Pellet& pellet, Map& map) {
    std::string key = generateKey(
        static_cast<int>(pellet.getPosition().x / map.getCellSize()),
        static_cast<int>(pellet.getPosition().y / map.getCellSize()));
    if (pellet.isActive()) {
        hashmap[key].push_back(&pellet);
    }
    else {
        if (hashmap.count(key)) {
            auto& cell = hashmap[key];
            cell.erase(std::remove(cell.begin(), cell.end(), &pellet), cell.end());
        }
    }
}

std::string Hashmap::generateKey(int x, int y) const {
    return std::to_string(x) + "," + std::to_string(y);
}

std::vector<Pellet*> Hashmap::getPelletsInSameCell(float x, float y, Map& map) const {
    std::string key = generateKey
        (static_cast<int>(x / map.getCellSize()), 
        static_cast<int>(y / map.getCellSize()));
    if (hashmap.count(key)) {
        return hashmap.at(key);
    }
    else {
        return std::vector<Pellet*>(); // return an empty vector if no key found
    }
}

std::vector<Pellet*> Hashmap::checkCollision(Circle& circle, Map& map) {
    std::vector<Pellet*> nearbyPellets = getPelletsInSameCell(circle.getPosition().x, circle.getPosition().y, map);
    std::vector<Pellet*> collidedPellets;
    for (Pellet* pelletPtr : nearbyPellets) {
        Pellet& pellet = *pelletPtr;
        float dx = pellet.getPosition().x - circle.getPosition().x;
        float dy = pellet.getPosition().y - circle.getPosition().y;
        float distanceSquared = dx * dx + dy * dy;
        float sumRadii = circle.getCirclesize() + pellet.getRadius();

        if (distanceSquared <= sumRadii * sumRadii) {
            collidedPellets.push_back(pelletPtr);
        }
    }
    return collidedPellets;
}
