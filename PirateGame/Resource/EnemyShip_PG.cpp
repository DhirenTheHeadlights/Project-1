#include "EnemyShip_PG.h"

using namespace PirateGame;

void EnemyShip::customShipSetUp() {
	// Add handlers
	inputHandler = std::make_unique<EnemyShipInputHandler>(getSprite());
	movementHandler = std::make_unique<EnemyShipMovementHandler>(getSprite());

	// Set a random pos
	getSprite().setPosition(GlobalValues::getInstance().getMap().getRandomPosition());
}

void EnemyShip::customShipUpdate() {
	inputHandler->update();
}

void EnemyShip::customShipDraw() {
	// Draw the health bar
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
	window->draw(healthBarRed);
	window->draw(healthBarGreen);
}