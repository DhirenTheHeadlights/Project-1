#include "PlayerShip_PG.h"

using namespace PirateGame;

/// Custom functions

void PlayerShip::customShipSetUp() {
	SIH = std::make_unique<PlayerShipInputHandler>(getSprite());
	SMH = std::make_unique<PlayerShipMovementHandler>(getSprite());
	SIvH = std::make_unique<ShipInventoryHandler>();

	SIvH->addGold(1000);

	SIH->setNumCannons(getShipProperties().numCannons);
	SIH->setBaseSpeed(getShipProperties().baseSpeed);
	SIH->setCannonHandler(&getCannonHandler());
	SIH->setMovementHandler(SMH.get());

	getSprite().setPosition(GlobalValues::getInstance().getMap().getRandomPosition());
}

void PlayerShip::customShipUpdate() {
	// Update the health bar
	healthBarGreen.setSize(sf::Vector2f(getHealth() / getShipProperties().maxHealth * healthBarRed.getSize().x, healthBarRed.getSize().y));
	healthBarGreen.setPosition(healthBarRed.getPosition());

	SIH->update();
}

void PlayerShip::customShipDraw() {
	// Draw the health bar
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
	window->draw(healthBarRed);
	window->draw(healthBarGreen);

	SIH->draw();
}