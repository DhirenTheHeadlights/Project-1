#pragma once
#include <SFML/Graphics.hpp>
#include "CellGroup.h"

class InputHandler {
public:
	InputHandler(sf::RenderWindow& window);
	void split(CellGroup& cellGroup);
	void splitLogic(CellGroup& cellGroup);
	float zoomWithScrollWheel(sf::Event event);
private:
	sf::RenderWindow& window;
	float zoomMin = 0.5f, zoomMax = 2.0f;
};
