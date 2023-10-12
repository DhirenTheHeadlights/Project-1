#include "World.h"

int addSize = 0;
Circle circle(5);
Map map;

World::World(sf::RenderWindow& window) : hashmap(map, window) {
    map.grid(1920, 1080, 5);
}

void World::createWorld(sf::RenderWindow& window) {
    circle.move(circle.getCirclesize(), 100, 5, window);
    removePelletWhenCollision();
    drawPellets(window);
    circle.draw(window);
    drawInformation(window);
}

void World::removePelletWhenCollision() {
    std::vector<Pellet*> collidedPellets = hashmap.checkCollision(circle, map); // Gets a vector of collided pellets
    bool hasCollided = false; // To track if at least one collision occurred
    for (Pellet* collidedPelletPtr : collidedPellets) {
        if (!collidedPelletPtr->isActive()) {
            continue; // Skip pellets that have already been deactivated in a previous collision
        }
        collidedPelletPtr->deActivate();
        if (!hasCollided) {
            growCircle(); // Apply the increase circle size function only once per frame
            hasCollided = true; // Set flag to true since a collision was detected
            printActivePellets();
        }
        collidedPelletPtr->printActiveStatus();
    }
    activePellets.erase( // Now, remove inactive pellets from activePellets vector
        std::remove_if(activePellets.begin(), activePellets.end(), [](Pellet& pellet) {
            return !pellet.isActive();
            }), activePellets.end()
    );
    if (hasCollided) {
        std::cout << "Collided with " << collidedPellets.size() << " pellets. addSize: " << addSize << std::endl;
    }
}

void World::growCircle() {
    map.setCellSize(5 * static_cast<float>(5 + addSize));
    addSize += 1;
    circle.setCircleSize(static_cast<float>(5 + addSize));
}

void World::drawPellets(sf::RenderWindow& window) {
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

std::string World::printActivePellets() const{ // For debugging
    std::string returnString;
    for (auto& pellet : activePellets) {
        std::string string = "Pellet at(" + std::to_string(pellet.getPosition().x) 
                                + ", " + std::to_string(pellet.getPosition().y) + ") ";
        if (pellet.isActive()) {
            returnString += string + "is ACTIVE";
        }
        else {
			returnString += string + "is INACTIVE";
		}
		returnString += "\n";
	}
	return returnString;
}

void World::drawInformation(sf::RenderWindow& window) {
    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile("times_new_roman.ttf")) {
        std::cerr << "Failed to load font\n";
    }
    else {
		text.setFont(font);
		text.setString(printActivePellets());
		text.setCharacterSize(12);
		text.setFillColor(sf::Color::Red);
		text.setPosition(10, 10);
		window.draw(text);
    }
}

int World::getCircleSize() const {
    return 5 + addSize;
}
