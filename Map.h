#pragma once
#include <iostream>

class Map {
public:
    void grid(int boardWidth, int boardHeight, int cellSize);
    int getCellSize() const;
private:
    int rows, cols, cellSize;
};