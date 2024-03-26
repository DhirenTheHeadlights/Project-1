#include "EnemyShipHandler_PG.h"

using namespace PirateGame;

void EnemyShipHandler::addEnemyShips(int numShipsPerChunk) {
	// Grab all chunks
	std::vector<Map*> maps = GlobalMap::getInstance().getAllChunks();

	for (auto& map : maps) {
		addEnemyShipsToChunk(*map, numShipsPerChunk);
	}
}

void EnemyShipHandler::addEnemyShipsToChunk(Map& map, int numShipsPerChunk) {
	// Grab positions for the ships
	std::vector<sf::Vector2f> points = map.getRandomPositions(minDistBetweenShips, numShipsPerChunk);

	// Add the ships
	for (int i = 0; i < points.size(); i++) {
		// Create a new ship
		std::shared_ptr<EnemyShip> ship = std::make_shared<EnemyShip>();

		// Set up the ship
		ship->setUpShip();
		ship->getCannonHandler().setCooldown(enemyCannonCooldown);
		ship->getMovementHandler().setTurningSpeed(turningSpeed);
		ship->getMovementHandler().setEnemySpeedMultiplier(enemySpeedMultiplier);
		ship->getInputHandler().setFiringDistance(firingDistance);
		ship->getMovementHandler().setAnchorDrop(true);
		ship->getSprite().setPosition(points[i]);

		// Here, the hashmap for the cannonballs is given to each ship. The hashmap is taken
		// from the GlobalHashmapHandler, which you would think wouldnt be necessary since the
		// hashmap is global, but it is necessary because to avoid circular dependencies, the
		ship->getCannonHandler().setCannonballHashmap(GlobalHashmapHandler::getInstance().getCannonballHashmap());

		// Add the ship to the vector, important to use std::move and
		// do after setting up the ship, otherwise the ship will be empty
		enemyShips.push_back(std::move(ship));
	}

	// Add the ships to the hashmap
	for (auto& ship : enemyShips) {
		GlobalHashmapHandler::getInstance().getShipHashmap()->addObject(ship.get());
	}
}

void EnemyShipHandler::update() {
	// Grab nearby ships for the player ship
	std::set<EnemyShip*> nearbyShips = GlobalHashmapHandler::getInstance().getShipHashmap()->findObjectsNearObject(playerShip, maxDetectionDistance);

	// Update all the enemy ships nearby the player
	for (auto& ship : nearbyShips) {
		if (ship->getHealth() < 0.01f) { // Temporary, will be replaced with a more robust system
			// Delete the ship from the hashmap
			GlobalHashmapHandler::getInstance().getShipHashmap()->removeObject(ship);

			// Delete the ship from the vector
			enemyShips.erase(std::remove_if(enemyShips.begin(), enemyShips.end(), [ship](std::shared_ptr<EnemyShip>& s) { return s.get() == ship; }), enemyShips.end());
		}
		ship->setActive(true);
		ship->getMovementHandler().setAnchorDrop(false);
		ship->setPlayerPosition(playerShip->getSprite().getPosition());
	}

	// Update all the enemy ships
	for (auto& ship : enemyShips) {
		GlobalHashmapHandler::getInstance().getShipHashmap()->updateObjectPosition(ship.get());
		ship->update();
	}
}

void EnemyShipHandler::draw() {
	// Draw all the enemy ships
	for (auto& ship : enemyShips) {
		ship->draw();
	}
}