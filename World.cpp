#include "World.h"

int addSize = 0;

Circle circle(5);

World::World(Map& map, sf::RenderWindow& window) : hashmap(map, window) {
    map.grid(1920, 1080, 5);
}

void World::createWorld(sf::RenderWindow& window, Map& map) {
    circle.move(circle.getCirclesize(), 100, 5, window);
    removePelletWhenCollision(circle, hashmap, map, window);
    drawPellets(window, map, hashmap);
    circle.draw(window);
    drawInformation(window);
}

void World::removePelletWhenCollision(Circle& circle, Hashmap& hashmap, Map& map, sf::RenderWindow& window) {
    std::vector<Pellet*> collidedPellets = hashmap.checkCollision(circle, map); // Gets a vector of collided pellets
    bool hasCollided = false; // To track if at least one collision occurred
    for (Pellet* collidedPelletPtr : collidedPellets) {
        if (!collidedPelletPtr->isActive()) {
            continue; // Skip pellets that have already been deactivated in a previous collision
        }
        collidedPelletPtr->deActivate();
        if (!hasCollided) {
            growCircle(map); // Apply the increase circle size function only once per frame
            hasCollided = true; // Set flag to true since a collision was detected
            printActivePellets();
        }
        collidedPelletPtr->printActiveStatus();
    }
    activePellets.erase( // Now, remove inactive pellets from activePellets vector
        std::remove_if(activePellets.begin(), activePellets.end(), [](Pellet& pellet) {
            return !pellet.isActive();
            }),
        activePellets.end()
    );
    if (hasCollided) {
        std::cout << "Collided with " << collidedPellets.size() << " pellets. addSize: " << addSize << std::endl;
    }
}



void World::growCircle(Map& map) {
    map.setCellSize(5 * static_cast<float>(5 + addSize));
    addSize += 1;
    circle.setCircleSize(static_cast<float>(5 + addSize));
}

void World::drawPellets(sf::RenderWindow& window, Map& map, Hashmap& hashmap) {
    if (activePellets.empty()) {
        for (int i = 0; i < 100; i++) {
            float x = std::rand() % window.getSize().x;
            float y = std::rand() % window.getSize().y;
            activePellets.emplace_back(x, y);
        }
    }
    for (auto& pellet : activePellets) {     // Iterate through active pellets
        pellet.draw(window);
        hashmap.assignPellet(pellet, map);
    }
}

void World::printActivePellets() const{ // For debugging
    for (auto& pellet : activePellets) {
        std::cout << "Pellet at (" << pellet.getPosition().x << ", " << pellet.getPosition().y << ") ";
        if (pellet.isActive()) {
            std::cout << "is active" << std::endl;
        }
        else {
            std::cout << "is inactive" << std::endl;
        }
    }
}

void World::drawInformation(sf::RenderWindow& window) {
    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile("times_new_roman.ttf")) {
        std::cerr << "Failed to load font\n";
    }
    else {
		text.setFont(font);
		text.setString("Size: " + std::to_string(circle.getCirclesize()));
		text.setCharacterSize(18);
		text.setFillColor(sf::Color::Red);
		text.setPosition(10, 10);
		window.draw(text);
    }
}

int World::getCircleSize() const {
    return 5 + addSize;
}
