#include <vector>
#include <iostream>
#include "Map.h"

void Map::grid(int boardWidth, int boardHeight, int cellSize) {
    rows = boardHeight / cellSize;
    cols = boardWidth / cellSize;
    gridData.resize(rows, std::vector<int>(cols, 0));  // Initialize all cells to 0
}

void Map::setCellValue(int row, int col, int value) {
    gridData[row][col] = value;
}

int Map::getCellValue(int row, int col) const {
    return gridData[row][col];
}

void Map::printGrid() const {
    for (const auto& row : gridData) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << '\n';
    }
}