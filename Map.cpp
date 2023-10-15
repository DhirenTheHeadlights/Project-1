#include <vector>
#include <iostream>
#include "Map.h"
#include "Globals.h"

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
    sf::RectangleShape line(sf::Vector2f(window.getSize().x, 1)); // Horizontal line
    line.setFillColor(sf::Color::White);
    for (int i = 0; i <= rows; i++) {
        line.setPosition(0, i * cellSize);
        window.draw(line);
    }
    line.setSize(sf::Vector2f(1, window.getSize().y)); // Vertical line
    for (int i = 0; i <= cols; i++) {
        line.setPosition(i * cellSize, 0);
        window.draw(line);
    }
}