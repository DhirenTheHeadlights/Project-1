#include "Button_PG.h"

using namespace PirateGame;

// Interact with the button
void Button::interact(sf::RenderWindow* window, GlobalSoundManager* GSM) {
	holdDown ? interactHold(window, GSM) : interactOnce(window, GSM);
}

void Button::interactOnce(sf::RenderWindow* window, GlobalSoundManager* GSM) const {
	if (Input::isMouseButtonPressedOnce(sf::Mouse::Left)) {
		sf::Vector2i mousePosition = sf::Vector2i(sf::Mouse::getPosition(*window));
		sf::Vector2f worldPosition = window->mapPixelToCoords(mousePosition);

		// If the mouse is over the button, call the button's callback function
		if (sprite.getGlobalBounds().contains(worldPosition)) {
			GSM->playSound(SoundId::Select);
			func();
		}
	}
}

void Button::interactHold(sf::RenderWindow* window, GlobalSoundManager* GSM) {
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	sf::Vector2f worldPosition = window->mapPixelToCoords(mousePosition);

	if (sprite.getGlobalBounds().contains(worldPosition) &&  // If the mouse is over the button
		cooldown.getElapsedTime().asSeconds() > cooldownTime.asSeconds() && // If the cooldown has passed
		sf::Mouse::isButtonPressed(sf::Mouse::Left) // If the left mouse button is pressed
		) {
		GSM->playSound(SoundId::Select);
		func();
		cooldown.restart();
	}
}