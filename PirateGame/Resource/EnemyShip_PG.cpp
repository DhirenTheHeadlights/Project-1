#include "EnemyShip_PG.h"

using namespace PirateGame;

void EnemyShip::customShipSetUp() {
	// Add handlers
	SIH = std::make_unique<EnemyShipInputHandler>(getSprite());
	SMH = std::make_unique<EnemyShipMovementHandler>(getSprite());

	SIH->setNumCannons(getShipProperties().numCannons);
	SIH->setBaseSpeed(getShipProperties().baseSpeed);
	SIH->setCannonHandler(getCannonHandler());
	SIH->setMovementHandler(SMH.get());
}

void EnemyShip::customShipUpdate() {
	SIH->update();
	SMH->move(getShipProperties().baseSpeed, SSH->getSailDirectionVector());
	getSailHandler()->update(getSprite());
}

void EnemyShip::customShipDraw() {
	// Draw the health bar
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
	window->draw(healthBarRed);
	window->draw(healthBarGreen);

	getCannonHandler()->drawCannons();
	getSailHandler()->draw();
}

