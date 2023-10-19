#include "World.h"

World::World(sf::RenderWindow& window) : hashmap(window), view(window.getDefaultView()) {
    map.grid(10000, 10000, 100);
    if (!font.loadFromFile("times_new_roman.ttf")) {
        std::cerr << "Failed to load font\n";
    }
}

void World::createWorld(sf::RenderWindow& window, sf::Event& event) {
    cellGroup.move(100, map, window);
    checkCollisionForEveryCell();
    drawPellets(window, 10000);
    map.drawGrid(window);

    if (gameOver) {
        drawInformation(window, "Game Over! Press R to restart", 50, view.getCenter().x, view.getCenter().y);
        return;
    }

    cellGroup.draw(window);
    Circle* cellPos = cellGroup.getCellGroup()[0];
    drawInformation(window, "Mass: " + std::to_string(cellPos->getCircleSize()), 10, cellPos->getPosition().x, cellPos->getPosition().y);
    drawVector(cellPos->getPosition(), cellPos->getDirection(), window, 100.0f, sf::Color::Red);

    view.setCenter(cellPos->getPosition().x, cellPos->getPosition().y);
    view.setSize(window.getDefaultView().getSize() * zoomMultiplier);

    if (event.type == sf::Event::MouseWheelScrolled) {
        zoomMultiplier *= (event.mouseWheelScroll.delta > 0) ? 0.99f : 1.01f;
        zoomMultiplier = std::clamp(zoomMultiplier, 0.5f, 2.0f);  // Assuming these as min and max values.
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        cellGroup.split();
    }

    window.setView(view);
}

void World::checkCollisionForEveryCell() {
    for (Circle* cell : cellGroup.getCellGroup()) removePelletWhenCollision(cell);
}


void World::removePelletWhenCollision(Circle* cell) {
    std::set<Pellet*> collidedPellets = hashmap.checkCollisionWithinBoundsOfCircle(cell, map);
    float numActiveCollisions = 0;  // to keep track of number of active collisions this frame
    for (Pellet* collidedPelletPtr : collidedPellets) {
        if (collidedPelletPtr->getRadius() > cell->getCircleSize()) {
            if (0.7 * collidedPelletPtr->getRadius() > cell->getCircleSize()) gameOver = true;
            else continue;
		}
        if (!collidedPelletPtr->isActive()) {
            continue; // Skip pellets that have already been deactivated in a previous collision
        }
        collidedPelletPtr->deActivate();
        numActiveCollisions += static_cast<float>(0.1 * collidedPelletPtr->getRadius());
    }
    activePellets.erase( // Remove all inactive pellets from the activePellets vector
        std::remove_if(activePellets.begin(), activePellets.end(), [](const std::shared_ptr<Pellet>& pelletPtr) {
            return !pelletPtr->isActive();}), activePellets.end());
    if (numActiveCollisions > 0) {
        if (addSize < maxSize) {
            addSize += numActiveCollisions;
        }
        cell->setCircleSize(startingCircleSize + addSize);
    }
}

void World::drawPellets(sf::RenderWindow& window, int numPellets) {
    if (activePellets.empty()) {
        for (int i = 0; i < numPellets; i++) {
            float x = static_cast<float>(std::rand() % map.getLength());
            float y = static_cast<float>(std::rand() % map.getLength());
            activePellets.push_back(std::make_shared<Pellet>(x, y));
        }
    }

    if (mainTime.getElapsedTime().asSeconds() > timeBetweenPelletsSpawning) {
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

void World::drawVector(const sf::Vector2f & start, const sf::Vector2f & direction, sf::RenderWindow & window, float magnitude, sf::Color color) {
    sf::VertexArray lines(sf::Lines, 2);

    lines[0].position = start; // start position
    lines[0].color = color;

    // calculate end position by adding the direction (normalized) multiplied by the magnitude
    lines[1].position = start + direction / std::sqrt(direction.x * direction.x + direction.y * direction.y) * magnitude;
    lines[1].color = color;

    window.draw(lines);
}

void World::restart() {
	gameOver = false;
	cellGroup.reset();
	addSize = 0;
}

bool World::isGameOver() const {
	return gameOver;
}