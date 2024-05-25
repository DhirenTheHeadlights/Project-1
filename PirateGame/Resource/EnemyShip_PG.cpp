#include "EnemyShip_PG.h"

using namespace PirateGame;

void EnemyShip::customShipSetUp() {
	// Add handlers
	SIH = std::make_unique<EnemyShipInputHandler>(getSprite());
	SMH = std::make_unique<EnemyShipMovementHandler>(getSprite());

	SIH->setNumCannons(getSpecificShipProperties().numCannons);
	SIH->setBaseSpeed(getSpecificShipProperties().baseSpeed);
	SIH->setCannonHandler(getCannonHandler());
	SIH->setMovementHandler(SMH.get());
	SIH->setSailHandler(getSailHandler());
}

void EnemyShip::customShipUpdate() {
	SIH->update();
	SMH->update(getSpecificShipProperties().baseSpeed, SSH->getAverageSailDirection());
}

void EnemyShip::customShipDraw() {
	// Draw the health bar
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
	window->draw(healthBarRed);
	window->draw(healthBarGreen);

	getCannonHandler()->drawCannons();
	getSailHandler()->draw();
}

