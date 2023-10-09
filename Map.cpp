#include <vector>
#include <iostream>
#include "Map.h"
#include "Globals.h"

void Map::grid(int boardLength, int boardHeight, int cellSize) {
    rows = boardHeight / cellSize;
    cols = boardLength / cellSize;
    this->cellSize = cellSize;
}

int Map::getCellSize() const {
    return cellSize;
}