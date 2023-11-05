#pragma once
#include <SFML/Graphics.hpp>
#include "CellGroup.h"

class InputHandler {
public:
	InputHandler(sf::RenderWindow& window);
	void split(CellGroup& cellGroup);
	void zoomWithScrollWheel(sf::Event event);
	float getZoomOffset() const;
private:
	sf::RenderWindow& window;
	float zoomMin = 0.5f, zoomMax = 2.0f;
	float zoomOffset = 1.0f;
};
