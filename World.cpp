#include "World.h"

int addSize = 0;

Circle circle(5);

World::World(Map& map, sf::RenderWindow& window) {
    map.grid(1920, 1080, 50);
}

void World::createWorld(sf::RenderWindow& window, Hashmap& hashmap, Map& map) {
    circle.move(circle.getCirclesize(), 100, 5, window);
    removePelletWhenCollision(circle, hashmap, map, window);
    drawPellets(window, map, hashmap);
    circle.draw(window);
}

void World::removePelletWhenCollision(Circle& circle, Hashmap& hashmap, Map& map, sf::RenderWindow& window) {
    std::vector<Pellet*> collidedPellets = hashmap.checkCollision(circle, hashmap, map, window);
    bool hasCollided = false;  // To track if at least one collision occurred
    for (Pellet* collidedPelletPtr : collidedPellets) {
        collidedPelletPtr->deActivate();
        hasCollided = true;  // Set flag to true since a collision was detected
    }
    if (hasCollided) {
        addSize += 1;
        circle.setCircleSize(static_cast<float>(5 + addSize));
        std::cout << "Collided with " << collidedPellets.size() << " pellets. addSize: " << addSize << std::endl;
    }

    // Now, remove inactive pellets from activePellets vector
    activePellets.erase(
        std::remove_if(activePellets.begin(), activePellets.end(), [](Pellet& pellet) {
            return !pellet.isActive();
            }),
        activePellets.end()
    );
}


void World::drawPellets(sf::RenderWindow& window, Map& map, Hashmap& hashmap) {
    if (activePellets.empty()) {
        for (int i = 0; i < 100; i++) {
            float x = std::rand() % window.getSize().x;
            float y = std::rand() % window.getSize().y;
            activePellets.emplace_back(x, y);
        }
    }
    for (auto& pellet : activePellets) {
        pellet.draw(window);
        hashmap.assignPellet(pellet, map);
    }
}

int World::getCircleSize() const {
    return 5 + addSize;
}
