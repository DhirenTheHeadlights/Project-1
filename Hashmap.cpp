#include "Hashmap.h"
#include "Globals.h"

void Hashmap::assignPellet(const Pellet& pellet) {
    float x = pellet.getPosition().x;
    float y = pellet.getPosition().y;
    int gridX = x / map.getCellSize();
    int gridY = y / map.getCellSize();
    std::string key = generateKey(gridX, gridY);
    GridObject gridObject{ pellet };
    hashmap[key].push_back(gridObject);
}

void Hashmap::assignCircle(const Circle& circle) {
    float x = circle.getPosition().x;
    float y = circle.getPosition().y;
    int gridX = x / map.getCellSize();
    int gridY = y / map.getCellSize();
    std::string key = generateKey(gridX, gridY);
    GridObject gridObject{ circle };
    hashmap[key].push_back(gridObject);
}

std::string Hashmap::generateKey(int x, int y) const{
    return std::to_string(x) + "," + std::to_string(y);
}

std::vector<Pellet> Hashmap::getPelletsInSameCell(float x, float y) const {
    int gridX = x / map.getCellSize();
    int gridY = y / map.getCellSize();
    std::string key = generateKey(gridX, gridY);
    auto it = hashmap.find(key);
    if (it != hashmap.end()) {
        std::vector<Pellet> pellets;
        for (const auto& gridObject : it->second) {
            if (auto pelletPtr = std::get_if<Pellet>(&gridObject.object)) {
                pellets.push_back(*pelletPtr);
            }
        }
        return pellets;
    }
    return {};
}

bool Hashmap::checkCollision(Circle& circle, Pellet& pellet, Hashmap hashmap) {
    float x = circle.getPosition().x;
    float y = circle.getPosition().y;
    std::vector<Pellet> nearbyPellets = hashmap.getPelletsInSameCell(x, y);
    for (const auto& pellet : nearbyPellets) {
        float dx = pellet.getPosition().x - x;
        float dy = pellet.getPosition().y - y;
        float distanceSquared = dx * dx + dy * dy;
        float sumRadii = circle.getCirclesize() + pellet.getRadius();
        if (distanceSquared <= sumRadii * sumRadii) {
            return true;
        }
        else {
            return false;
        }
    }
}

