#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

#include <VectorMath.h>

class Map {
public:
    // This is the initial constructor for the map
    void initialize(int boardLength, int boardHeight, int cellSize, sf::Vector2f position = sf::Vector2f(0.f, 0.f));

    // Draw the grid
    void drawGrid(sf::RenderWindow& window) const;

    // Setters
    void setCellSize(int cellSize) { this->cellSize = cellSize; }

    // Getters
    std::pair<int, int> getGridCoordinates(float x, float y) const { return { static_cast<int>(x + position.x) / cellSize, static_cast<int>(y + position.y) / cellSize }; }
    int getCellSize() const { return cellSize; }
    int getLength() const { return len; }
    std::vector<sf::Vector2f> const getRandomPositions(float minDistance = 500.f, int numPoints = 1) const;
    sf::Vector2f getRandomPosition() const { return sf::Vector2f(vm::randomValue(position.x, position.x + static_cast<float>(len)), vm::randomValue(position.y, position.y + static_cast<float>(height))); }
    sf::FloatRect getBounds() const { return sf::FloatRect(position, sf::Vector2f(static_cast<float>(len), static_cast<float>(height))); }
	bool mapContains(sf::Vector2f position) const { return getBounds().contains(position); }

private:
    int rows = 1, cols = 1, cellSize = 1;
    int len = 1, height = 1;
    sf::Vector2f position;
};