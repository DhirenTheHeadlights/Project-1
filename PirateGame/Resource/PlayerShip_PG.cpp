#include "PlayerShip_PG.h"

using namespace PirateGame;

/// Custom functions

void PlayerShip::customShipSetUp() {
	SIH = std::make_unique<PlayerShipInputHandler>(getSprite(), context.GSM.get(), context.GIH.get(), context.GWC.get());
	SMH = std::make_unique<PlayerShipMovementHandler>(context.GV->getWindow(), getSprite(), getSpecificShipProperties().baseSpeed);
	
	SIvH = std::make_unique<ShipInventoryHandler>();
	SIvH->addGold(1000);
	
	SIH->setBaseSpeed(getSpecificShipProperties().baseSpeed);
	SIH->setCannonHandler(getCannonHandler());
	SIH->setMovementHandler(SMH.get());
	SIH->setSailHandler(getSailHandler());

	getInputHandler()->setInAudioRange(true);
}

void PlayerShip::customShipUpdate() {
	// If the experience is greater than the experience to level up
	// Level up the ship and also up the amount of exp to level up
	if (experience > experienceToLevelUp) {
		experience = 0.f;
		experienceToLevelUp *= 2;
		playerLevel += 1;
	}
}

void PlayerShip::customShipDraw() {
	// Draw the health bar
	sf::RenderWindow* window = context.GV->getWindow();
	
	getCannonHandler()->drawCannons(window);
	getSailHandler()->draw(window);
}