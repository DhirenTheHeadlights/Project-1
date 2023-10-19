#include "CellGroup.h"

CellGroup::CellGroup(const float startSize, Map& map) {
	x = std::rand() % map.getLength();
	y = std::rand() % map.getLength();
	Circle* cell = new Circle(startSize);
	cell->setPosition(x, y);
	cells.push_back(cell);
}

CellGroup::~CellGroup() {
	for (Circle* cell : cells) {
		delete cell;
	}
}

void CellGroup::split() {
	std::vector<Circle*> newCells;
	for (Circle* cell : cells) {
		float newSize = cell->getCircleSize() / 2.0f;
		if (newSize < minSize || cells.size() >= 16) { // prevent cell from getting too small
			continue;
		}
		cell->setCircleSize(newSize);
		Circle* newCell = new Circle(newSize);
		newCell->setPosition(cell->getPosition().x + newSize, cell->getPosition().y + newSize);
		newCells.push_back(newCell);
	}
	cells.insert(cells.end(), newCells.begin(), newCells.end());
}

void CellGroup::removeCircle(Circle& cell, Map& map) {
	auto it = std::find_if(cells.begin(), cells.end(), [&](const Circle* c) { return *c == cell; });
	if (it != cells.end()) {
		delete* it; // Deallocate memory
		cells.erase(it);
	}
}

void CellGroup::draw(sf::RenderWindow& window) {
	for (Circle* cell : cells) cell->draw(window);
}

void CellGroup::move(double moveSpeed, Map& map, sf::RenderWindow& window) {
	for (Circle* cell : cells) {
		if (!collision()) cell->move(cell->getCircleSize(), moveSpeed, map, window);
	}
}

bool CellGroup::collision() {
	for (auto it1 = cells.begin(); it1 != cells.end(); ++it1) {
		for (auto it2 = it1 + 1; it2 != cells.end(); ++it2) {
			if ((*it1)->checkCollision(**it2)) return true;
		}
	}
}

void CellGroup::reset() {
	cells.clear();
	numSplits = 1;
	numCells = 1;
}

std::vector<Circle*> CellGroup::getCellGroup() {
	return cells;
}