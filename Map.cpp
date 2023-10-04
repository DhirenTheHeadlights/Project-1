#include <vector>
#include <iostream>
#include "Map.h"
#include "Globals.h"

void Map::grid(int boardWidth, int boardHeight, int cellSize) {
    rows = boardHeight / cellSize;
    cols = boardWidth / cellSize;
    this->cellSize = cellSize;
}

int Map::getCellSize() const {
    return cellSize;
}