#include "Hashmap_AG.h"

Hashmap::Hashmap(sf::RenderWindow& window) {
}

std::string Hashmap::generateKey(int x, int y) const {
    return std::to_string(x) + "," + std::to_string(y);
}

void Hashmap::assignPellet(Pellet& pellet, Map& map) {
    std::string key = generateKey(
        static_cast<int>(pellet.getPosition().x / map.getCellSize()),
        static_cast<int>(pellet.getPosition().y / map.getCellSize()));

    if (pellet.isActive()) pelletHashmap[key].insert(&pellet);
    else pelletHashmap[key].erase(&pellet);
}

std::set<Pellet*> Hashmap::getPelletsInSameCell(float x, float y) const {
    std::string key = generateKey(x, y);
    if (pelletHashmap.count(key)) return pelletHashmap.at(key);
    return {};
}

void Hashmap::assignCell(Circle& cell, Map& map) {
    std::string key = generateKey(
        static_cast<int>(cell.getPosition().x / map.getCellSize()),
        static_cast<int>(cell.getPosition().y / map.getCellSize()));
    cellHashmap[key].insert(&cell);
}

std::set<Circle*> Hashmap::getCellsInSameCell(Circle* circle, Map& map) const {
    std::set<Circle*> cellsNearby;

    float left = circle->getPosition().x - circle->getCircleSize();
    float right = circle->getPosition().x + circle->getCircleSize();
    float top = circle->getPosition().y - circle->getCircleSize();
    float bottom = circle->getPosition().y + circle->getCircleSize();

    auto topLeft = map.getGridCoordinates(left, top);
    auto bottomRight = map.getGridCoordinates(right, bottom);

    for (int i = topLeft.first; i <= bottomRight.first; i++) {
        for (int j = topLeft.second; j <= bottomRight.second; j++) {
            std::string key = generateKey(i, j);
            if (cellHashmap.find(key) != cellHashmap.end()) {
                const std::set<Circle*>& cellSet = cellHashmap.at(key);
                for (Circle* nearbyCell : cellSet) {
                    if (nearbyCell != circle) {  // Avoid adding the circle itself
                        cellsNearby.insert(nearbyCell);
                    }
                }
            }
        }
    }
    return cellsNearby;
}

const std::unordered_map<std::string, std::set<Pellet*>>& Hashmap::getPelletHashmap() const {
	return pelletHashmap;
}

const std::unordered_map<std::string, std::set<Circle*>>& Hashmap::getCellHashmap() const {
	return cellHashmap;
}