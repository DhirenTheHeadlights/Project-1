#include "Hashmap.h"

Hashmap::Hashmap(sf::RenderWindow& window) : window(window) {
}

void Hashmap::assignPellet(Pellet& pellet, Map& map) {
    std::string key = generateKey(
        static_cast<int>(pellet.getPosition().x / map.getCellSize()),
        static_cast<int>(pellet.getPosition().y / map.getCellSize()));

    if (pellet.isActive()) hashmap[key].insert(&pellet);
    else hashmap[key].erase(&pellet);
}

std::string Hashmap::generateKey(int x, int y) const {
    return std::to_string(x) + "," + std::to_string(y);
}

std::set<Pellet*> Hashmap::getPelletsInSameCell(float x, float y) const {
    std::string key = generateKey(x, y);
    if (hashmap.count(key)) return hashmap.at(key);
    return {};
}

std::set<Pellet*> Hashmap::checkCollisionWithinBoundsOfCircle(Circle* circle, Map& map) {
    std::set<Pellet*> collidedPellets;
    float left = circle->getPosition().x - circle->getCircleSize();
    float right = circle->getPosition().x + circle->getCircleSize();
    float top = circle->getPosition().y - circle->getCircleSize();
    float bottom = circle->getPosition().y + circle->getCircleSize();
    auto topLeft = map.getGridCoordinates(left, top);
    auto bottomRight = map.getGridCoordinates(right, bottom);
    for (int i = topLeft.first; i <= bottomRight.first; i++) {
        for (int j = topLeft.second; j <= bottomRight.second; j++) {
            std::string key = generateKey(i, j);
            checkCollision(key, circle, collidedPellets);
            drawCell(i, j, map, sf::Color::White);
        }
    }
    return collidedPellets;
}

void Hashmap::checkCollision(const std::string& key, Circle* circle, std::set<Pellet*>& collidedPellets) {
    if (hashmap.find(key) != hashmap.end()) {
        for (Pellet* pellet : hashmap[key]) {
            float dx = pellet->getPosition().x - circle->getPosition().x;
            float dy = pellet->getPosition().y - circle->getPosition().y;
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance < (pellet->getRadius() * minDistForCollision + circle->getCircleSize())) collidedPellets.insert(pellet);
        }
    }
}

void Hashmap::drawCell(int x, int y, Map& map, sf::Color color) {
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(map.getCellSize(), map.getCellSize()));
    rectangle.setFillColor(color);
    rectangle.setPosition(x * map.getCellSize(), y * map.getCellSize());
    window.draw(rectangle);
}
