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

	// If the experience is greater than the experience to level up
	// Level up the ship and also up the amount of exp to level up
	if (experience > experienceToLevelUp) {
		experience = 0.f;
		experienceToLevelUp *= 2;
	}
}

void PlayerShip::customShipDraw() {
	// Draw the health bar
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
	SIH->draw();
}