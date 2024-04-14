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
		
		// Here, the hashmap for the cannonballs is given to each ship. The hashmap is taken
		// from the GlobalHashmapHandler, which you would think wouldnt be necessary since the
		// hashmap is global, but it is necessary because to avoid circular dependencies, the
		ship->getCannonHandler().setCannonballHashmap(GlobalHashmapHandler::getInstance().getCannonballHashmap());

		// Add the ship to a new ship group and vector, std::move is not needed
		// do after setting up the ship, otherwise the ship will be empty
		std::shared_ptr<ShipGroup> group = std::make_shared<ShipGroup>();
		ship->setGroupID(group->getID());	
		group->addShip(ship);
		setShipGroupDestination(group);
		enemyShips.push_back(std::move(ship));
		shipGroups.push_back(std::move(group));
	}

	// Add the ships to the hashmap
	for (auto& ship : enemyShips) {
		GlobalHashmapHandler::getInstance().getShipHashmap()->addObject(ship.get());
	}
}

void EnemyShipHandler::setShipGroupDestination(std::shared_ptr<ShipGroup> group) {
	sf::Vector2f position = landmasses[std::rand() % landmasses.size()]->getSprite().getPosition();
	group->setHeading(position);
}

bool EnemyShipHandler::isDestinationReached(std::shared_ptr<ShipGroup> shipGroup) {
	// Get the average position of the ships in the group
	sf::Vector2f averagePosition = shipGroup->getAveragePosition();

	// Calculate the distance between the average position and the target
	float distance = GlobalValues::getInstance().distanceBetweenPoints(averagePosition, shipGroup->getHeading());

	// If the distance is less than the destination reached distance, return true
	if (distance < destinationReachedDistance) return true;
	return false; // Otherwise, return false
}

void EnemyShipHandler::update() {
	// Grab nearby ships for the player ship
	std::set<EnemyShip*> nearbyShips = GlobalHashmapHandler::getInstance().getShipHashmap()->findObjectsNearObject(playerShip, maxDetectionDistance);

	//// Update all the enemy ships groups near the player ship
	//for (auto& ship : nearbyShips) {
	//	// Grab the ship group ID
	//	int groupID = ship->getGroupID();

	//	// Look for the ship group
	//	auto it = std::find_if(shipGroups.begin(), shipGroups.end(), [groupID](std::shared_ptr<ShipGroup> group) { return group->getID() == groupID; });

	//	// Set the target for the ship group. We dont need to check if there is a group, since the ship should always have a group
	//	(*it)->setTarget(playerShip->getSprite().getPosition());
	//	(*it)->setInCombat(true);
	//}

	// Update all the enemy ships
	for (auto& enemyShipGroup : shipGroups) {
		// Check if the destination is reached
		if (isDestinationReached(enemyShipGroup)) {
			// Set a new destination
			setShipGroupDestination(enemyShipGroup);
		}
		enemyShipGroup->updateGroup();
	}


	// Ship groups edited by random chance when 2 enemy ship groups are close to each other
	// and will be destroyed when they are far away from each other. By default, there is 1 ship in each group.
	// The ship groups will be used to calculate the flocking behavior of the enemy ships.
	for (auto& enemyShipGroup : shipGroups) {
		if (enemyShipGroup == nullptr) continue;

		// If the group is size 0, remove it
		if (enemyShipGroup->getEnemyShips().size() == 0) {
			// Remove the ship group from the vector
			shipGroups.erase(std::remove(shipGroups.begin(), shipGroups.end(), enemyShipGroup), shipGroups.end());
			std::cout << "Ship group removed. Group size: " << shipGroups.size() << std::endl;
			continue;
		}

		// Grab nearby ships, for now from just the first ship in the group
		std::set<EnemyShip*> nearbyShips = GlobalHashmapHandler::getInstance().getShipHashmap()->findObjectsNearObject(enemyShipGroup->getEnemyShips()[0].get(), maxDetectionDistance);

		for (auto& otherShip : nearbyShips) { // For each nearby ship
			// Skip ships that are in the same group
			if (otherShip->getGroupID() == enemyShipGroup->getID()) continue;

			// Check if the ship has already been interacted with
			if (enemyShipGroup->isGroupIDInteractedWith(otherShip->getGroupID())) continue;

			// Otherwise, roll a coin to see if the ship should be added to the group. 1 is a grouping, 2 is an attack, all other values are no interaction.
			std::uniform_int_distribution<int> dist(0, interactionChance);
			int interaction = dist(GlobalValues::getInstance().getRandomEngine());

			// Shows if there is interaction. Possible framework for future attack indicator!
			GlobalValues::getInstance().displayText(std::to_string(otherShip->getID()) + ", Interact = " + std::to_string(interaction), otherShip->getSprite().getPosition() + sf::Vector2f(25, 25), (interaction != 1 && interaction != 2) ? sf::Color::White : sf::Color::Red, 20);

			if (interaction == 1) {
				// Find the other ship in the vector, this is a workaround since the getNearbyObjects function returns a raw pointer
				auto it = std::find_if(enemyShips.begin(), enemyShips.end(), [otherShip](std::shared_ptr<EnemyShip> ship) { return ship.get() == otherShip; });
				enemyShipGroup->addShip(*it); // Add the ship to the group

				// Remove the ship from its old group
				for (auto& group : shipGroups) {
					if (group->getID() == otherShip->getGroupID()) {
						group->removeShip(*it);
					}
				}

				(*it)->setGroupID(enemyShipGroup->getID()); // Set the group ID for the ship
				enemyShipGroup->addGroupIDInteractedWith(otherShip->getGroupID()); // Add the group ID to the list of groups interacted with

				continue;
			}
			else if (1 == 1) {
				// Otherwise, fight the other ship group
				auto otherShipGroup = std::find_if(shipGroups.begin(), shipGroups.end(), [otherShip](std::shared_ptr<ShipGroup> group) { return group->getID() == otherShip->getGroupID(); });
				otherShipGroup->get()->addTarget(enemyShipGroup->getEnemyShips()[std::rand() % static_cast<int>(enemyShipGroup->getEnemyShips().size())].get());
				otherShipGroup->get()->setInCombat(true);

				// Set the target for the ship group
				enemyShipGroup->addTarget(otherShip);
				enemyShipGroup->setTargetVelocity(otherShip->getMovementHandler().getVelocity());
				enemyShipGroup->setInCombat(true);

				// Add the ship to the list of ships interacted with
				enemyShipGroup->addGroupIDInteractedWith(otherShip->getGroupID());
			}

			// Check each ship in combat to see if it is still in nearbyShips. If not, remove it from ShipIDsCombatting
			int shipIDindex = 0;
			for (auto& i : enemyShipGroup->getShipIDsCombatting()) {
				auto shipStillInCombat = std::find_if(nearbyShips.begin(), nearbyShips.end(), [i](EnemyShip* ship) { return ship->getID() == i; });
				if (shipStillInCombat == nearbyShips.end()) {
					enemyShipGroup->getShipIDsCombatting().erase(enemyShipGroup->getShipIDsCombatting().begin() + shipIDindex);
				}
				shipIDindex++;
			}

			// If there are no more ships in combat, set inCombat to false
			if (enemyShipGroup->getShipIDsCombatting().size() == 0) {
				enemyShipGroup->setInCombat(false);
			}
		}
	}
}

void EnemyShipHandler::draw() {
	// Draw all the enemy ship groups
	for (auto& enemyShipGroup : shipGroups) {
		enemyShipGroup->drawGroup();
	}
}