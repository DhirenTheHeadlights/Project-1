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

	// Set the text origin to top-left before positioning
	text.setOrigin(0, 0);

	// Center the text within the sprite
	sf::FloatRect spriteBounds = sprite.getGlobalBounds();
	sf::FloatRect textBounds = text.getLocalBounds();
	text.setPosition(spriteBounds.left + (spriteBounds.width - textBounds.width) / 2.0f - textBounds.left,
		spriteBounds.top + (spriteBounds.height - textBounds.height) / 2.0f - textBounds.top);
}