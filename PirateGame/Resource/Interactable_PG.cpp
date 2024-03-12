#include "Interactable_PG.h"

using namespace PirateGame;

void Interactable::createInteractable(sf::Texture& texture, sf::Text& title, sf::Vector2f scale) {
	this->text = title;
	this->texture = texture;
	this->name = title.getString();
	sprite.setTexture(texture);
	sprite.setScale(scale);

	customInteractableSetUp();
}

void Interactable::draw() {
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
	window->draw(sprite);
	window->draw(text);
}

void Interactable::setPosition(sf::Vector2f pos) {
	sprite.setPosition(pos);
	// Set the text to be in the center of the sprite
	text.setPosition(sf::Vector2f(pos.x + sprite.getGlobalBounds().getSize().x / 2 - text.getGlobalBounds().getSize().x / 2, 
								  pos.y + sprite.getGlobalBounds().getSize().y / 2 - text.getGlobalBounds().getSize().y / 2));
}