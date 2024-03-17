#include "EnemyShipHandler_PG.h"

using namespace PirateGame;

void EnemyShipHandler::addEnemyShips(int numShips) {
	// Add the ships
	for (int i = 0; i < numShips; i++) {
		std::shared_ptr<EnemyShip> ship = std::make_shared<EnemyShip>();
		enemyShips.push_back(std::move(ship));
		enemyShips[i]->setUpShip();
		enemyShips[i]->getCannonHandler().setCooldown(enemyCannonCooldown);
		enemyShips[i]->getMovementHandler().setTurningSpeed(turningSpeed);
		enemyShips[i]->getMovementHandler().setEnemySpeedMultiplier(enemySpeedMultiplier);
		enemyShips[i]->getInputHandler().setFiringDistance(firingDistance);
		enemyShips[i]->getMovementHandler().setAnchorDrop(true);
	}
	// Add the ships to the hashmap
	for (auto& ship : enemyShips) {
		GlobalHashmapHandler::getInstance().getShipHashmap()->addEnemyShip(ship.get());
	}
}

void EnemyShipHandler::update() {

	// Grab nearby ships for the player ship
	std::set<EnemyShip*> nearbyShips = GlobalHashmapHandler::getInstance().getShipHashmap()->findEnemyShipsNearShip(playerShip, maxDetectionDistance);

	// Update all the enemy ships nearby the player
	for (auto& ship : nearbyShips) {
		ship->setActive(true);
		ship->getMovementHandler().setAnchorDrop(false);
		ship->setPlayerPosition(playerShip->getSprite().getPosition());
	}

	// Update all the enemy ships
	for (auto& ship : enemyShips) {
		GlobalHashmapHandler::getInstance().getShipHashmap()->updateEnemyShipPosition(ship.get());
		ship->update();
	}
}

void EnemyShipHandler::draw() {
	// Draw all the enemy ships
	for (auto& ship : enemyShips) {
		ship->draw();
	}
}