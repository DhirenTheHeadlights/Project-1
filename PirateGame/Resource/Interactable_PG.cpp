#include "Interactable_PG.h"

using namespace PirateGame;

void Interactable::createInteractable(sf::Vector2f size) {
	this->size = size;

	// Set the size of all the interactable objects
	background.setSize(size);
	frame.setSize(size);
	foreground.setSize(sf::Vector2f(size.x - padding * 2, size.y - padding * 2));
	text.setCharacterSize(static_cast<unsigned int>(size.y / 3));
	text.setFont(font);

	// Set the color of the interactable objects
	updateColors();
}

void Interactable::draw() {
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
	window->draw(background);
	window->draw(foreground);
	window->draw(frame);
	window->draw(text);
}

void Interactable::updateColors() {
	background.setFillColor(backgroundColor);
	foreground.setFillColor(foregroundColor);
	frame.setFillColor(frameColor);
	frame.setOutlineColor(frameColor);
	frame.setOutlineThickness(outlineThickness);
	text.setFillColor(textColor); 
}

void Interactable::setPosition(sf::Vector2f pos) {
	this->position = pos;
	// Set the position of the interactable objects
	background.setPosition(position);
	frame.setPosition(position);
	foreground.setPosition(sf::Vector2f(position.x + padding, position.y + padding));
	// Set the text to be in the center of the foreground
	float x = foreground.getPosition().x + foreground.getSize().x / 2 - text.getGlobalBounds().width / 2;
	float y = foreground.getPosition().y + foreground.getSize().y / 2 - text.getGlobalBounds().height / 2;
	text.setPosition(sf::Vector2f(x, y));
}