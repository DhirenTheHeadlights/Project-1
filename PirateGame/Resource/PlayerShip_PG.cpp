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
}

void PlayerShip::customShipUpdate() {
	SIH->update();
}

void PlayerShip::customShipDraw() {
	// Draw the health bar
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
	SIH->draw();
}