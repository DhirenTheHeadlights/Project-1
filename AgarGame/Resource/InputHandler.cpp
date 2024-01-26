#include "InputHandler.h"

InputHandler::InputHandler(sf::RenderWindow& window) : window(window) {
}

void InputHandler::split(CellGroup& cellGroup) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		cellGroup.split();
	}
}

void InputHandler::zoomWithScrollWheel(sf::Event event) {
	if (event.type == sf::Event::MouseWheelScrolled) {
		zoomOffset *= (event.mouseWheelScroll.delta > 0) ? 0.99f : 1.01f;
		zoomOffset = std::clamp(zoomOffset, zoomMin, zoomMax);
	}
}

float InputHandler::getZoomOffset() const {
	return zoomOffset;
}