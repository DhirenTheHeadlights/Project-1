#include "EnemyShip_PG.h"


using namespace PirateGame;

void EnemyShip::customShipSetUp() {
	// Add handlers
	SIH = std::make_unique<EnemyShipInputHandler>(context.JSL.get(), getSprite(), context.GSM.get());
	SMH = std::make_unique<EnemyShipMovementHandler>(getSprite(), getSpecificShipProperties().baseSpeed, context.JSL.get());

	SIH->setBaseSpeed(getSpecificShipProperties().baseSpeed);
	SIH->setCannonHandler(getCannonHandler());
	SIH->setMovementHandler(SMH.get());
	SIH->setSailHandler(getSailHandler());
}

void EnemyShip::customShipUpdate() {
}

void EnemyShip::customShipDraw() {
	// Draw the health bar
	sf::RenderWindow* window = Globals::window;
	window->draw(healthBarRed);
	window->draw(healthBarGreen);

	getCannonHandler()->drawCannons(window);
	getSailHandler()->draw(window);

	if (debug) {
		TextQueue::displayText("Health: " + std::to_string(getSpecificShipProperties().maxHealth), getSprite().getPosition() + sf::Vector2f(0, 50), sf::Color::White);
	}
}

