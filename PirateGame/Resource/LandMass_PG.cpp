#include "LandMass_PG.h"

void LandMass::createLandMass(LandMassType type, Textures& texture) {
	// Set the type of landmass
	this->type = type;

	// Case switch for the type of landmass
	switch (type) {
		case LandMassType::Island:
			selectIslandSprite(texture);
			break;
		case LandMassType::Rock:
			selectRockSprite(texture);
			break;
		case LandMassType::Shipwreck:
			break;
	}
}

// Functions to select and assign sprites
void LandMass::selectIslandSprite(Textures& texture) {
	sprite.setTexture(texture.grabIslandTexture());
	sprite.setScale(1.f * scaling, 1.f * scaling);
}
void LandMass::selectRockSprite(Textures& texture) {

	sprite.setTexture(texture.grabRockTexture());
	sprite.setScale(0.125 * scaling, 0.125 * scaling);
}

// Set the position of the land mass
void LandMass::setPosition(sf::Vector2f pos) {
	interactablePoint.setPosition(pos);
	sprite.setPosition(pos);
}

// Draw the land mass
void LandMass::draw(sf::RenderWindow& window) {
	window.draw(interactablePoint);
	window.draw(sprite);
}

// Add an item to the market
void LandMass::addItem(std::string name, int price) {
	// Add item to the landmass
	market.push_back(std::make_pair(name, price));
}
