#include "Button_PG.h"

using namespace PirateGame;

// Interact with the button
void Button::interact() {
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();

	// If the mouse button is not pressed, return
	if (!GlobalValues::getInstance().getInputHandler().isMouseButtonPressedOnce(sf::Mouse::Left)) {
		return;
	}

	// Get the mouse position
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*window));

	// If the mouse is over the button, call the button's callback function
	if (foreground.getGlobalBounds().contains(mousePos)) {
		func();
	}
}