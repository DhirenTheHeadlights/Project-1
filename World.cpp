#include "World.h"

float addSize = 0;
Circle circle(5);
Map map;
double collidedPellets_size = 0;

World::World(sf::RenderWindow& window) : hashmap(map, window) {
    map.grid(10000, 10000, 100);
}

void World::createWorld(sf::RenderWindow& window) {
    circle.move(circle.getCircleSize(), 100, 5, map, window);
    removePelletWhenCollision(window);
    drawPellets(window, 10000);
    circle.draw(window);
    map.drawGrid(window);
    drawInformation(window, std::to_string(circle.getPosition().x) + ", " + std::to_string(circle.getPosition().y), 10);

    // Set the view's center to the circle's position
    sf::View view(window.getView());
    view.setCenter(circle.getPosition().x + circle.getCircleSize(), circle.getPosition().y + circle.getCircleSize());

    // Adjust the view's size if needed (e.g., for zooming)
    view.setSize(window.getDefaultView().getSize() * (static_cast<float>(0.1) * log(circle.getCircleSize())));

    // Set the modified view to the window
    window.setView(view);

    //to debug
    if (map.getLength() != 10000) {
		std::cout << "Map length is not 10000" << std::endl;
    }
}


void World::removePelletWhenCollision(sf::RenderWindow& window) {
    std::set<Pellet*> collidedPellets = hashmap.checkCollision(circle, map);
    float numActiveCollisions = 0;  // to keep track of number of active collisions this frame
    for (Pellet* collidedPelletPtr : collidedPellets) {
        if (!collidedPelletPtr->isActive()) {
            continue; // Skip pellets that have already been deactivated in a previous collision
        }
        collidedPelletPtr->deActivate();
        numActiveCollisions += (0.1 * collidedPelletPtr->getRadius());
    }
    activePellets.erase( // Now, remove inactive pellets from activePellets vector
        std::remove_if(activePellets.begin(), activePellets.end(), [](Pellet& pellet) {
            return !pellet.isActive(); }), activePellets.end());
    if (numActiveCollisions > 0) {
        growCircle(numActiveCollisions);
        collidedPellets_size = numActiveCollisions; // For debugging
        std::cout << circle.getCircleSize() << ", " << numActiveCollisions << std::endl;
    }
}

void World::growCircle(float numCollisions) {
    float growthAmount = numCollisions; // adjust growth based on numCollisions
    if (addSize < 100) {
        addSize += growthAmount;
	}
    circle.setCircleSize(static_cast<float>(5 + addSize));
}

void World::drawPellets(sf::RenderWindow& window, int numPellets) { // Draws pellets
    if (activePellets.empty()) {
        for (int i = 0; i < numPellets; i++) {
            float x = std::rand() % map.getLength();
            float y = std::rand() % map.getLength();
            activePellets.emplace_back(x, y);
        }
    }
    for (auto& pellet : activePellets) {     // Iterate through active pellets
        pellet.draw(window);
        hashmap.assignPellet(pellet, map);
    }
}

void World::drawInformation(sf::RenderWindow& window, std::string info, int textSize) { // For debugging
    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile("times_new_roman.ttf")) {
        std::cerr << "Failed to load font\n";
    }
    else {
		text.setFont(font);
		text.setString(info);
		text.setCharacterSize(textSize);
		text.setFillColor(sf::Color::Red);
		text.setPosition(circle.getPosition().x, circle.getPosition().y);
		window.draw(text);
    }
}

int World::getCircleSize() const {
    return 5 + addSize;
}
