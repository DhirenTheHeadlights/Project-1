#pragma once
#include <vector>
#include <iostream>

class Map {
public:
    void grid(int boardWidth, int boardHeight, int cellSize);
    void setCellValue(int row, int col, int value);
    int getCellValue(int row, int col) const;
    void printGrid() const;
private:
    int rows;
    int cols;
    std::vector<std::vector<int>> gridData;
};