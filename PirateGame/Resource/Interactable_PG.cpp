#include "Interactable_PG.h"

using namespace PirateGame;

void Interactable::createInteractable(sf::Texture& texture, sf::Text& title, sf::Vector2f scale) {
	this->text = title;
	this->texture = texture;
	this->name = title.getString();
	sprite.setTexture(texture);
	sprite.setScale(scale);
	sprite.setOrigin(sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));

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
	text.setPosition(sf::Vector2f(pos.x - text.getGlobalBounds().width / 2, pos.y - text.getGlobalBounds().height / 2));
}