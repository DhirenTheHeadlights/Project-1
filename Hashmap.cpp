#include "Hashmap.h"

Hashmap::Hashmap(Map& map, sf::RenderWindow& window) {
}

void Hashmap::assignPellet(Pellet& pellet, Map& map) {
    float x = pellet.getPosition().x;
    float y = pellet.getPosition().y;
    int gridX = x / map.getCellSize();
    int gridY = y / map.getCellSize();
    std::string key = generateKey(gridX, gridY);
    hashmap[key].push_back(&pellet); 
}

void Hashmap::assignCircle(Circle& circle, Map& map) {
    float x = circle.getPosition().x;              
    float y = circle.getPosition().y;             
    int gridX = x / map.getCellSize();            
    int gridY = y / map.getCellSize();           
    std::string key = generateKey(gridX, gridY);            
    hashmap[key].push_back(&circle);           
}

std::string Hashmap::generateKey(int x, int y) const {
    return std::to_string(x) + "," + std::to_string(y);
}

std::vector<Pellet*> Hashmap::getPelletsInSameCell(float x, float y, Map& map) const {
    int gridX = x / map.getCellSize();
    int gridY = y / map.getCellSize();
    std::string key = generateKey(gridX, gridY);
    std::vector<Pellet*> pellets;
    if (hashmap.count(key)) {
        for (const auto& variant : hashmap.at(key)) {
            if (auto pelletPtr = std::get_if<Pellet*>(&variant)) {
                pellets.push_back(*pelletPtr);
            }
        }
    }
    return pellets;
}


std::vector<Pellet*> Hashmap::checkCollision(Circle& circle, Hashmap& hashmap, Map& map, sf::RenderWindow& window) {
    float x = circle.getPosition().x;
    float y = circle.getPosition().y;
    std::vector<Pellet*> nearbyPellets = hashmap.getPelletsInSameCell(x, y, map);
    std::vector<Pellet*> collidedPellets;

    for (Pellet* pelletPtr : nearbyPellets) {
        Pellet& pellet = *pelletPtr;
        float dx = pellet.getPosition().x - x;
        float dy = pellet.getPosition().y - y;
        float distanceSquared = dx * dx + dy * dy;
        float sumRadii = circle.getCirclesize() + pellet.getRadius();

        if (distanceSquared <= sumRadii * sumRadii) {
            collidedPellets.push_back(pelletPtr);
        }
    }
    return collidedPellets;
}