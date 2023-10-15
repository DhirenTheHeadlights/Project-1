#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class Map {
public:
    void grid(int boardLength, int boardHeight, int cellSize);
    void setCellSize(int cellSize);
    void drawGrid(sf::RenderWindow& window);
    std::pair<int, int> getGridCoordinates(float x, float y) const;
    int getCellSize() const;
    int getLength() const;
private:
    int rows, cols, cellSize;
};