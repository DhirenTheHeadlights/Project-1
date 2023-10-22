#include "World.h"

World::World(sf::RenderWindow& window) : 
    colMan(window, map), view(window.getDefaultView()), 
    pelletHandler(window) {
    if (!font.loadFromFile("times_new_roman.ttf")) {
        std::cerr << "Failed to load font\n";
    }
    map.grid(10000, 10000, 100);
}

void World::createWorld(sf::RenderWindow& window, sf::Event& event) {
    colMan.checkPelletCollisionForEveryCell(window, cellGroup, map, pelletHandler.getActivePellets());
    pelletHandler.drawPellets(window, map, 10000);
    map.drawGrid(window);
    cellGroup.draw(window);
    cellGroup.move(100, map, window);

    if (gameOver) {
        drawInformation(window, "Game Over! Press R to restart", 50, view.getCenter().x, view.getCenter().y);
        return;
    }

    avgX = 0.0f;
    avgY = 0.0f;

    colMan.assignCells(cellGroup, map);
    colMan.assignPellets(pelletHandler.getActivePellets(), map);
    colMan.separateAllCells(cellGroup, map);
    pelletHandler.eraseIfInactive();

    for (Circle* cell : cellGroup.getCellGroup()) {
        cell->setColor(sf::Color::Blue);
        drawVector(cell->getPosition(), cell->getDirection(), window, 100.0f, sf::Color::Red);
        avgX += cell->getPosition().x;
        avgY += cell->getPosition().y;
        drawInformation(window, "Mass: " + std::to_string(cell->getCircleSize()), 10, cell->getPosition().x, cell->getPosition().y);
    }

    avgX /= cellGroup.getCellGroup().size();
    avgY /= cellGroup.getCellGroup().size();

    view.setCenter(avgX, avgY);
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

void World::restart() {
    gameOver = false;
    cellGroup.reset();
}

bool World::isGameOver() const {
    return gameOver;
}

void World::drawInformation(sf::RenderWindow& window, const std::string& info, int textSize, float setPosX, float setPosY) {
    sf::Text text;
    text.setFont(font);  // Use the preloaded font
    text.setString(info);
    text.setCharacterSize(textSize); // in pixels, not points!
    text.setFillColor(sf::Color::Red);

    // Center the text based on its local bounds
    float offsetX = text.getLocalBounds().width / 2.0f;
    float offsetY = text.getLocalBounds().height / 2.0f;

    text.setPosition(setPosX - offsetX, setPosY - offsetY);

    window.draw(text);

    std::cout << "Drawing text\n";
}

void World::drawVector(const sf::Vector2f& start, const sf::Vector2f& direction, sf::RenderWindow& window, float magnitude, sf::Color color) {
    sf::VertexArray lines(sf::Lines, 2);

    lines[0].position = start; // start position
    lines[0].color = color;

    // calculate end position by adding the direction (normalized) multiplied by the magnitude
    lines[1].position = start + direction / std::sqrt(direction.x * direction.x + direction.y * direction.y) * magnitude;
    lines[1].color = color;

    window.draw(lines);

    std::cout << "Drawing vector\n";
}