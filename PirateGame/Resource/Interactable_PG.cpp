#include "Interactable_PG.h"

using namespace PirateGame;

// Create a new interactable object
void Interactable::addInteractable(sf::Vector2f& size) {
	// Set the size of all the interactable objects
	background.setSize(size);
	foreground.setSize(sf::Vector2f(size.x - padding * 2, size.y - padding * 2));

	// Set the color of the interactable objects
	background.setFillColor(backgroundColor);
	foreground.setFillColor(foregroundColor);
	frame.setFillColor(frameColor);
	text.setFillColor(textColor);

	// Set the position of the interactable objects
	background.setPosition(position);
	foreground.setPosition(sf::Vector2f(position.x + padding, position.y + padding));
	text.setPosition(sf::Vector2f(position.x + padding, position.y + padding));
}

// Draw the interactable object
void Interactable::draw(sf::RenderWindow& window) {
	window.draw(background);
	window.draw(foreground);
	window.draw(frame);
	window.draw(text);
}

// Update the colors of the interactable objects
void Interactable::updateColors() {
	background.setFillColor(backgroundColor);
	foreground.setFillColor(foregroundColor);
	frame.setFillColor(frameColor);
	text.setFillColor(textColor);
}