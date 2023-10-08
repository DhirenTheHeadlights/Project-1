#include "World.h"

std::vector<sf::Color> colors = {
sf::Color::Red,
sf::Color::Green,
sf::Color::Blue,
sf::Color::Yellow,
sf::Color::Cyan,
sf::Color::Magenta,
sf::Color::White };

Hashmap hashmap;

World::World() {
    Circle circle(30.f);
    circle.move(circle.getCirclesize(), 0.01, 2);
    World::drawPellets(window);
    circle.draw(window);
    hashmap.checkCollision(circle, hashmap);
}

void World::drawPellets(sf::RenderWindow& window) {
    if (pellets.empty()) {  // Create pellets only once
        for (int i = 0; i < 100; i++) {
            float x = std::rand() % window.getSize().x;
            float y = std::rand() % window.getSize().y;
            pellets.emplace_back(x, y);
        }
    }
    for (auto& pellet : pellets) {
        pellet.draw(window);
        hashmap.assignPellet(pellet);
    }
}
