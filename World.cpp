#include "World.h"

std::vector<sf::Color> colors = {
sf::Color::Red,
sf::Color::Green,
sf::Color::Blue,
sf::Color::Yellow,
sf::Color::Cyan,
sf::Color::Magenta,
sf::Color::White };

void World::drawPellets(sf::RenderWindow& window, Hashmap hashmap) {
    if (pellets.empty()) {  // Create pellets only once
        for (int i = 0; i < 100; i++) {
            float x = std::rand() % SCREEN_WIDTH;
            float y = std::rand() % SCREEN_HEIGHT;
            pellets.emplace_back(x, y);
        }
    }
    for (auto& pellet : pellets) {
        pellet.draw(window);
        hashmap.assignPellet(pellet);
    }
}

void World::RemovePelletWhenCollision(std::vector) {
 
}