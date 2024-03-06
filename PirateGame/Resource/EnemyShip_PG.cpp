#include "EnemyShip_PG.h"

using namespace PirateGame;

void EnemyShip::customShipSetUp() {
	// implement later
}

void EnemyShip::customShipUpdate() {
	// implement later
}

void EnemyShip::customShipDraw() {
	// Draw the health bar
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
	window->draw(healthBarRed);
	window->draw(healthBarGreen);
}