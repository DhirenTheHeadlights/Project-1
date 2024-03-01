#include "LandMass_PG.h"

using namespace PirateGame;

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
	//setRandomRotation();
}

void LandMass::setRandomRotation() {
	sprite.setRotation(rand() % 360);
}

void LandMass::selectIslandSprite(Textures& texture) {
	sprite.setTexture(texture.grabIslandTexture());
	sprite.setScale(1.f * scaling, 1.f * scaling);
}
void LandMass::selectRockSprite(Textures& texture) {
	sprite.setTexture(texture.grabRockTexture());
	sprite.setScale(0.125f * scaling, 0.125f * scaling);
}

void LandMass::setPosition(sf::Vector2f pos) {
	interactablePoint.setPosition(pos);
	sprite.setPosition(pos);
}

void LandMass::draw(sf::RenderWindow& window) {
	window.draw(interactablePoint);
	window.draw(sprite);
}

void LandMass::addItemToMarket(std::string name, int price) {
	// Add item to market
	market.push_back(std::make_pair(name, price));
}
