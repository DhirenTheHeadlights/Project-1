#include "Button_PG.h"

using namespace PirateGame;

// Interact with the button
void Button::interact() {
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();

	if (!GlobalValues::getInstance().getInputHandler().isMouseButtonPressedOnce(sf::Mouse::Left)) {
		return;
	}

	sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(*window));

	// If the mouse is over the button, call the button's callback function
	if (foreground.getGlobalBounds().contains(mousePosition)) {
		func();
	}
}