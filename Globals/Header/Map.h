#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <optional>

class Map {
public:
    // This is the initial constructor for the map
    void grid(int boardLength, int boardHeight, int cellSize);

    // Setters
    void setCellSize(int cellSize);

    // Draw the grid
    void drawGrid(sf::RenderWindow& window) const;

    // Getters
    std::pair<int, int> getGridCoordinates(float x, float y) const;
    int getCellSize() const;
    int getLength() const;
    std::vector<sf::Vector2f> const getRandomPositions(float minDistance = 500.f, int numPoints = 1);
    sf::Vector2f getRandomPosition() {
        // Generate a random position
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.0f, 1.0f); // Ensure the distribution uses float
        // Explicitly cast len and height to float to avoid conversion warnings
        return sf::Vector2f(dis(gen) * static_cast<float>(len), dis(gen) * static_cast<float>(height));
    }
private:
    int rows = 1, cols = 1, cellSize = 1;
    int len = 1, height = 1;
};