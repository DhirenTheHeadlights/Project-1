#include "Button_PG.h"

using namespace PirateGame;

// Interact with the button
void Button::interact(sf::RenderWindow& window) {
	// If the mouse button is not pressed, return
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		return;
	}

	// Get the mouse position
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

	// If the mouse is over the button, call the button's callback function
	if (!Interactable::getForeground().getGlobalBounds().contains(mousePos)) {
		func();
	}
}