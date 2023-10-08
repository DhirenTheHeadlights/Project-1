#include "World.h"

Circle circle(30.f);

World::World(Map& map, sf::RenderWindow& window) {
    map.grid(10000, 10000, 10);  // Initialize the map
}

void World::drawCircle(sf::RenderWindow& window, Hashmap& hashmap, Map& map) {
    circle.move(circle.getCirclesize(), 0.01, 2, window);
    hashmap.checkCollision(circle, hashmap, map, window);
    circle.draw(window);
}

void World::drawPellets(sf::RenderWindow& window, Map& map, Hashmap& hashmap) {
    if (pellets.empty()) {  // Create pellets only once
        for (int i = 0; i < 100; i++) {
            float x = std::rand() % window.getSize().x;
            float y = std::rand() % window.getSize().y;
            pellets.emplace_back(x, y);
        }
    }
    for (auto& pellet : pellets) {
        pellet.draw(window);
        hashmap.assignPellet(pellet, map);
    }
}
