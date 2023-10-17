#include "Map.h"

void Map::grid(int boardLength, int boardHeight, int cellSize) {
    rows = boardHeight / cellSize;
    cols = boardLength / cellSize;
    this->cellSize = cellSize;
}

void Map::setCellSize(int cellSize) {
	this->cellSize = cellSize;
}

int Map::getCellSize() const {
    return cellSize;
}

void Map::drawGrid(sf::RenderWindow& window) {
    sf::RectangleShape line(sf::Vector2f(static_cast<float>(getLength()), 1.f)); // Horizontal line
    line.setFillColor(sf::Color::Green);
    for (int i = 0; i <= rows; i++) {
        line.setPosition(0, static_cast<float>(i * cellSize));
        window.draw(line);
    }
    line.setSize(sf::Vector2f(1.f, static_cast<float>(getLength()))); // Vertical line
    for (int i = 0; i <= cols; i++) {
        line.setPosition(static_cast<float>(i * cellSize), 0);
        window.draw(line);
    }
}

int Map::getLength() const {
	return rows * cellSize;
}

std::pair<int, int> Map::getGridCoordinates(float x, float y) const {
    return { static_cast<int>(x / cellSize), static_cast<int>(y / cellSize) };
}
