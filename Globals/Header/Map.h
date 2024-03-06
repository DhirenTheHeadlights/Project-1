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
    void drawGrid(sf::RenderWindow& window);

    // Getters
    std::pair<int, int> getGridCoordinates(float x, float y) const;
    int getCellSize() const;
    int getLength() const;
    std::vector<sf::Vector2f> getRandomPositions(float minDistance = 500.f, int numPoints = 1);
    sf::Vector2f getRandomPosition() {
        // Generate a random position
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        return sf::Vector2f(dis(gen) * len, dis(gen) * height);
    }
private:
    int rows = 1, cols = 1, cellSize = 1;
    int len = 1, height = 1;
};