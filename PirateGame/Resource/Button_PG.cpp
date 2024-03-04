#include "Button_PG.h"

using namespace PirateGame;

// Interact with the button
void Button::interact() {

	if (!GlobalValues::getInstance().getInputHandler().isMouseButtonPressedOnce(sf::Mouse::Left)) {
		return;
	}

	sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(*window));
	sf::Vector2f worldPosition = window->mapPixelToCoords(sf::Vector2i(mousePosition.x, mousePosition.y));

	// If the mouse is over the button, call the button's callback function
	if (foreground.getGlobalBounds().contains(worldPosition)) {
		func();
	}
}