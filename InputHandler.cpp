#include "InputHandler.h"

InputHandler::InputHandler(sf::RenderWindow& window) : window(window) {
}


void InputHandler::split(CellGroup& cellGroup) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		splitLogic(cellGroup);
	}
}

void InputHandler::splitLogic(CellGroup& cellGroup) {
	std::vector<Circle*> cells = cellGroup.getCellGroup();
	std::vector<Circle*> newCells;
	for (Circle* cell : cells) {
		float newSize = cell->getCircleSize() / 2.0f;
		if (newSize < cellGroup.getMinSize() || cells.size() >= 16) { // prevent cell from getting too small
			continue;
		}
		cell->setCircleSize(newSize);
		Circle* newCell = new Circle(newSize);

		newCell->setPosition(cell->getPosition().x + newSize, cell->getPosition().y + newSize);
		newCells.push_back(newCell);
	}
	cells.insert(cells.end(), newCells.begin(), newCells.end());
}

float InputHandler::zoomWithScrollWheel(sf::Event event) {
	float zoomOffset = 0.0f;
	if (event.type == sf::Event::MouseWheelScrolled) {
		zoomOffset *= (event.mouseWheelScroll.delta > 0) ? 0.99f : 1.01f;
		zoomOffset = std::clamp(zoomOffset, zoomMin, zoomMax);
	}
	return zoomOffset;
}