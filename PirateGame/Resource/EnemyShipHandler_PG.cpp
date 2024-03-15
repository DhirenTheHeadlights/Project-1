#include "EnemyShipHandler_PG.h"

using namespace PirateGame;

void EnemyShipHandler::addEnemyShips(int numShips) {
	// Add the ships
	for (int i = 0; i < numShips; i++) {
		std::shared_ptr<EnemyShip> ship = std::make_shared<EnemyShip>();
		shipHashmap.addEnemyShip(ship.get());
		enemyShips.push_back(std::move(ship));
		enemyShips[i]->setUpShip();
	}
}

void EnemyShipHandler::update() {
	// Grab nearby ships for the player ship
	std::set<EnemyShip*> nearbyShips = shipHashmap.findEnemyShipsNearShip(playerShip);

	// Update all the enemy ships nearby the player
	for (auto& ship : nearbyShips) {
		ship->update();
		ship->getMovementHandler().setPlayerPosition(playerShip->getSprite().getPosition());
	}
}

void EnemyShipHandler::draw() {
	// Draw all the enemy ships
	for (auto& ship : enemyShips) {
		ship->draw();
	}
}