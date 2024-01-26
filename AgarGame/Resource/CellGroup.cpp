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
	sf::Vector2f viewPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	for (Circle* cell : cells) {
		sf::Vector2f dirToMouse = viewPos - cell->getPosition();
		cell->move(moveSpeed, dirToMouse, map, window);
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

const float CellGroup::getMinSize() const {
	return minSize;
}

const sf::Vector2f CellGroup::getAvgPos() const {
	sf::Vector2f avgPos;
	for (Circle* cell : cells) {
		avgPos.x += cell->getPosition().x;
		avgPos.y += cell->getPosition().y;
	}
	avgPos.x /= cells.size();
	avgPos.y /= cells.size();

	return avgPos;
}