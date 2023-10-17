#include "World.h"

Circle circle(5);

World::World(sf::RenderWindow& window) : hashmap(map, window), view(window.getDefaultView()) {
    map.grid(10000, 10000, 100);
    // Load font here for efficiency
    if (!font.loadFromFile("times_new_roman.ttf")) {
        std::cerr << "Failed to load font\n";
    }
}

void World::createWorld(sf::RenderWindow& window, sf::Event& event) {
    circle.move(circle.getCircleSize(), 100, 5, map, window);
    removePelletWhenCollision(window);
    drawPellets(window, 10000);
    map.drawGrid(window);
    circle.draw(window);
    drawInformation(window, "Mass: " + std::to_string(circle.getCircleSize()), 10);
    view.setCenter(circle.getPosition().x + circle.getCircleSize(), circle.getPosition().y + circle.getCircleSize());  // Set the view's center to the circle's position
    view.setSize(window.getDefaultView().getSize() * zoomMultiplier * (static_cast<float>(0.2) * log(circle.getCircleSize())));  // Adjust the view's size if needed (e.g., for zooming)
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0) {
            // Zoom out
            zoomMultiplier *= 0.99f;
        }
        else {
            // Zoom in
            zoomMultiplier *= 1.01f;
        }
    }
    window.setView(view);
}


void World::removePelletWhenCollision(sf::RenderWindow& window) {
    std::set<Pellet*> collidedPellets = hashmap.checkCollisionWithinBoundsOfCircle(circle, map);
    float numActiveCollisions = 0;  // to keep track of number of active collisions this frame
    for (Pellet* collidedPelletPtr : collidedPellets) {
        if (collidedPelletPtr->getRadius() > circle.getCircleSize()) {
			continue; // Skip pellets that are bigger than the circle
		}
        if (!collidedPelletPtr->isActive()) {
            continue; // Skip pellets that have already been deactivated in a previous collision
        }
        collidedPelletPtr->deActivate();
        numActiveCollisions += (static_cast<float>(0.1 * collidedPelletPtr->getRadius()));
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
    if (addSize < 22500) {
        addSize += growthAmount;
	}
    circle.setCircleSize(static_cast<float>(5 + addSize));
}

void World::drawPellets(sf::RenderWindow& window, int numPellets) { // Draws pellets
    float x = static_cast<float>(std::rand() % map.getLength());
    float y = static_cast<float>(std::rand() % map.getLength());
    if (activePellets.empty()) {
        for (int i = 0; i < numPellets; i++) {
            activePellets.emplace_back(x, y);
        }
    }
    if (mainTime.getElapsedTime().asSeconds() > 0.1) {
        activePellets.emplace_back(x, y);
		mainTime.restart();
	}
    for (auto& pellet : activePellets) {     // Iterate through active pellets
        pellet.draw(window);
        hashmap.assignPellet(pellet, map);
    }
}

void World::drawInformation(sf::RenderWindow& window, std::string info, int textSize) {
    sf::Text text;
    text.setFont(font);  // Use the preloaded font
    text.setString(info);
    text.setCharacterSize(textSize);
    text.setFillColor(sf::Color::Red);
    text.setPosition(circle.getPosition().x - circle.getCircleSize(), circle.getPosition().y);
    window.draw(text);
}

int World::getCircleSize() const {
    return 5.f + addSize;
}