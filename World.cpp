#include "World.h"

int addSize = 0;
Circle circle(5);
Map map;
double collidedPellets_size = 0;

World::World(sf::RenderWindow& window) : hashmap(map, window) {
    map.grid(1920, 1080, 20);
}

void World::createWorld(sf::RenderWindow& window) { // Creates the world
    circle.move(circle.getCirclesize(), 100, 5, window);
    removePelletWhenCollision(window);
    drawPellets(window);
    circle.draw(window);
    //drawInformation(window, std::to_string(collidedPellets_size) + " pellets collided");
    map.drawGrid(window);
    drawInformation(window, std::to_string(circle.getPosition().x) + ", " + std::to_string(circle.getPosition().y));
}

void World::removePelletWhenCollision(sf::RenderWindow& window) {
    std::set<Pellet*> collidedPellets = hashmap.checkCollision(circle, map);
    int numActiveCollisions = 0;  // to keep track of number of active collisions this frame
    for (Pellet* collidedPelletPtr : collidedPellets) {
        //collidedPelletPtr->printActiveStatus();
        if (!collidedPelletPtr->isActive()) {
            continue; // Skip pellets that have already been deactivated in a previous collision
        }
        collidedPelletPtr->deActivate();
        numActiveCollisions++;
    }
    activePellets.erase( // Now, remove inactive pellets from activePellets vector
        std::remove_if(activePellets.begin(), activePellets.end(), [](Pellet& pellet) {
            return !pellet.isActive(); }), activePellets.end());
    if (numActiveCollisions > 0) {
        growCircle(numActiveCollisions);
        collidedPellets_size = numActiveCollisions; // For debugging
    }
}

void World::growCircle(int numCollisions) {
    int growthAmount = 1 * numCollisions; // adjust growth based on numCollisions
    //map.setCellSize(static_cast<float>(5 + addSize));
    if (addSize < 100) {
        addSize += growthAmount;
	}
    circle.setCircleSize(static_cast<float>(5 + addSize));
}

void World::drawPellets(sf::RenderWindow& window) { // Draws pellets
    if (activePellets.empty()) {
        for (int i = 0; i < 1000; i++) {
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

void World::drawInformation(sf::RenderWindow& window, std::string info) { // For debugging
    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile("times_new_roman.ttf")) {
        std::cerr << "Failed to load font\n";
    }
    else {
		text.setFont(font);
		text.setString(info);
		text.setCharacterSize(20);
		text.setFillColor(sf::Color::Red);
		text.setPosition(10, 10);
		window.draw(text);
    }
}

int World::getCircleSize() const {
    return 5 + addSize;
}
