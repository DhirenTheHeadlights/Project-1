#include "Button_PG.h"

using namespace PirateGame;

// Interact with the button
void Button::interact() {
	if (holdDown) {
		interactHold();
	}
	else {
		interactOnce();
	}
}

void Button::interactOnce() {
	if (GlobalValues::getInstance().getInputHandler().isMouseButtonPressedOnce(sf::Mouse::Left)) {
		sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(*window));
		sf::Vector2f worldPosition = window->mapPixelToCoords(sf::Vector2i(mousePosition.x, mousePosition.y));

		// If the mouse is over the button, call the button's callback function
		if (sprite.getGlobalBounds().contains(worldPosition)) {
			func();
		}
	}
}

void Button::interactHold() {
	sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(*window));
	sf::Vector2f worldPosition = window->mapPixelToCoords(sf::Vector2i(mousePosition.x, mousePosition.y));

	if (sprite.getGlobalBounds().contains(worldPosition) &&  // If the mouse is over the button
		cooldown.getElapsedTime().asSeconds() > cooldownTime.asSeconds() && // If the cooldown has passed
		sf::Mouse::isButtonPressed(sf::Mouse::Left) // If the left mouse button is pressed
		) {
		func();
		cooldown.restart();
	}
}