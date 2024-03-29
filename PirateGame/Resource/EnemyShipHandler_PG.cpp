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
		ship->getSprite().setPosition(points[i]);
		setShipDestination(ship.get());

		// Here, the hashmap for the cannonballs is given to each ship. The hashmap is taken
		// from the GlobalHashmapHandler, which you would think wouldnt be necessary since the
		// hashmap is global, but it is necessary because to avoid circular dependencies, the
		ship->getCannonHandler().setCannonballHashmap(GlobalHashmapHandler::getInstance().getCannonballHashmap());

		// Add the ship to a new ship group, important to use std::move and
		// do after setting up the ship, otherwise the ship will be empty
		std::shared_ptr<ShipGroup> group = std::make_shared<ShipGroup>();
		ship->setGroupID(group->getID());	
		group->addShip(ship);
	}

	// Add the ships to the hashmap
	for (auto& ship : enemyShips) {
		GlobalHashmapHandler::getInstance().getShipHashmap()->addObject(ship.get());
	}
}

void EnemyShipHandler::setShipDestination(EnemyShip* ship) {
	sf::Vector2f position = landmasses[std::rand() % landmasses.size()]->getSprite().getPosition();
	ship->getMovementHandler().setTravelDirection(position);
}

bool EnemyShipHandler::isDestinationReached(ShipGroup* shipGroup) {
	// Get the average position of the ships in the group
	sf::Vector2f averagePosition = shipGroup->getAveragePosition();

	// Calculate the distance between the average position and the target
	float distance = GlobalValues::getInstance().distanceBetweenPoints(averagePosition, shipGroup->getEnemyShips()[0]->getSprite().getPosition());

	// If the distance is less than the destination reached distance, return true
	if (distance < destinationReachedDistance) return true;
	return false; // Otherwise, return false
}

void EnemyShipHandler::update() {
	// Grab nearby ships for the player ship
	std::set<EnemyShip*> nearbyShips = GlobalHashmapHandler::getInstance().getShipHashmap()->findObjectsNearObject(playerShip, maxDetectionDistance);

	// Update all the enemy ships groups near the player ship
	for (auto& ship : nearbyShips) {
		if (ship->getHealth() < 0.01f) { // Temporary, will be replaced with a more robust system
			// Delete the ship from the hashmap
			GlobalHashmapHandler::getInstance().getShipHashmap()->removeObject(ship);

			// Delete the ship from the vector
			enemyShips.erase(std::remove_if(enemyShips.begin(), enemyShips.end(), [ship](std::shared_ptr<EnemyShip>& s) { return s.get() == ship; }), enemyShips.end());
		}
		ship->getMovementHandler().setIsActiveTowardsTarget(true);
		ship->setTargetPosition(playerShip->getSprite().getPosition());
	}

	// Update all the enemy ships
	for (auto& enemyShipGroup : shipGroups) {
		// Check if the destination is reached
		if (isDestinationReached(enemyShipGroup.get())) {
			// Set a new destination
			setShipDestination(enemyShipGroup->getEnemyShips()[0].get());
		}
		enemyShipGroup->updateGroup();
	}


	// Ship groups edited by random chance when 2 enemy ship groups are close to each other
	// and will be destroyed when they are far away from each other. By default, there is 1 ship in each group.
	// The ship groups will be used to calculate the flocking behavior of the enemy ships.
	for (auto& enemyShipGroup : shipGroups) {
		// Grab nearby ships, for now from just the first ship in the group
		std::set<EnemyShip*> nearbyShips = GlobalHashmapHandler::getInstance().getShipHashmap()->findObjectsNearObject(enemyShipGroup->getEnemyShips()[0].get(), minDistBetweenShips);

		for (auto& ship : enemyShipGroup->getEnemyShips()) { // For each ship in the group

			for (auto& otherShip : nearbyShips) { // For each nearby ship

				// Skip ships in the same group
				if (nearbyShips.find(ship.get()) != nearbyShips.end()) continue;

				// Otherwise, roll a coin to see if the ship should be added to the group
				std::uniform_int_distribution<int> dist(0, 1);

				if (dist(GlobalValues::getInstance().getRandomEngine()) == 1) {
					// Add the ship to the group
					enemyShipGroup->addShip(ship);
					ship->setGroupID(enemyShipGroup->getID());
					continue;
				}

				// Otherwise, fight the ship
				ship->getMovementHandler().setIsActiveTowardsTarget(true);
				ship->setTargetPosition(ship->getSprite().getPosition());

				// Set the target for the ship group
				enemyShipGroup->setTarget(ship->getSprite().getPosition());
				enemyShipGroup->setInCombat(true);
			}
		}
	}
}

void EnemyShipHandler::draw() {
	// Draw all the enemy ships
	for (auto& ship : enemyShips) {
		ship->draw();
	}
}