#include "World.h"

#include "World.h"

World::World(sf::RenderWindow& window) : hashmap(window), view(window.getDefaultView()) {
    map.grid(10000, 10000, 100);
    if (!font.loadFromFile("times_new_roman.ttf")) {
        std::cerr << "Failed to load font\n";
    }
}

void World::createWorld(sf::RenderWindow& window, sf::Event& event) {
    circle.move(circle.getCircleSize(), 100, map, window);
    removePelletWhenCollision(window);
    drawPellets(window, 10000);
    map.drawGrid(window);

    if (gameOver) {
        drawInformation(window, "Game Over! Press R to restart", 50, view.getCenter().x, view.getCenter().y);
        return;
    }

    circle.draw(window);
    drawInformation(window, "Mass: " + std::to_string(circle.getCircleSize()), 10, circle.getPosition().x - circle.getCircleSize(), circle.getPosition().y);

    view.setCenter(circle.getPosition().x + circle.getCircleSize(), circle.getPosition().y + circle.getCircleSize());
    view.setSize(window.getDefaultView().getSize() * zoomMultiplier * (0.2f * log(circle.getCircleSize())));

    if (event.type == sf::Event::MouseWheelScrolled) {
        zoomMultiplier *= (event.mouseWheelScroll.delta > 0) ? 0.99f : 1.01f;
    }

    window.setView(view);
}

void World::removePelletWhenCollision(sf::RenderWindow& window) {
    std::set<Pellet*> collidedPellets = hashmap.checkCollisionWithinBoundsOfCircle(circle, map);
    float numActiveCollisions = 0;  // to keep track of number of active collisions this frame
    for (Pellet* collidedPelletPtr : collidedPellets) {
        if (collidedPelletPtr->getRadius() > circle.getCircleSize()) {
            gameOver = true; // Game over if a pellet is larger than the circle
			continue; 
		}
        if (!collidedPelletPtr->isActive()) {
            continue; // Skip pellets that have already been deactivated in a previous collision
        }
        collidedPelletPtr->deActivate();
        numActiveCollisions += (static_cast<float>(0.1 * collidedPelletPtr->getRadius()));
    }
    activePellets.erase( // Remove all inactive pellets from the activePellets vector
        std::remove_if(activePellets.begin(), activePellets.end(), [](const std::shared_ptr<Pellet>& pelletPtr) {
            return !pelletPtr->isActive();}), activePellets.end());
    if (numActiveCollisions > 0) {
        growCircle(numActiveCollisions);
        collidedPelletsSize = numActiveCollisions; // For debugging
    }
}

void World::growCircle(float numCollisions) {
    float growthAmount = numCollisions; // adjust growth based on numCollisions
    if (addSize < 2250) {
        addSize += growthAmount;
	}
    circle.setCircleSize(static_cast<float>(5 + addSize));
}

void World::drawPellets(sf::RenderWindow& window, int numPellets) {
    if (activePellets.empty()) {
        for (int i = 0; i < numPellets; i++) {
            float x = static_cast<float>(std::rand() % map.getLength());
            float y = static_cast<float>(std::rand() % map.getLength());
            activePellets.push_back(std::make_shared<Pellet>(x, y));
        }
    }

    if (mainTime.getElapsedTime().asSeconds() > 0.1) {
        float x = static_cast<float>(std::rand() % map.getLength());
        float y = static_cast<float>(std::rand() % map.getLength());
        activePellets.push_back(std::make_shared<Pellet>(x, y));
        mainTime.restart();
    }

    for (const auto& pelletPtr : activePellets) {
        pelletPtr->draw(window);
        hashmap.assignPellet(*pelletPtr, map);
    }
}


void World::drawInformation(sf::RenderWindow& window, const std::string& info, int textSize, float setPosX, float setPosY) {
    sf::Text text;
    text.setFont(font);  // Use the preloaded font
    text.setString(info);
    text.setCharacterSize(textSize);
    text.setFillColor(sf::Color::Red);

    // Center the text based on its local bounds
    float offsetX = text.getLocalBounds().width / 2.0f;
    float offsetY = text.getLocalBounds().height / 2.0f;

    text.setPosition(setPosX - offsetX, setPosY - offsetY);

    window.draw(text);
}


void World::restart() {
	gameOver = false;
	circle.setCircleSize(startingCircleSize);
	addSize = 0;
}

bool World::isGameOver() const {
	return gameOver;
}