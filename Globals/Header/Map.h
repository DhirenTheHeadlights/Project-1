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
    std::optional<sf::Vector2f> getRandomPosition(float spacing);
private:
    int rows = 1, cols = 1, cellSize = 1;
    int len = 1, height = 1;

    // For disk sampling
    std::vector<sf::Vector2f> samplePoints;
    std::vector<sf::Vector2f> activeList;
};