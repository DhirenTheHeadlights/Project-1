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
		addEnemyShip(points[i]);
	}
}

void EnemyShipHandler::addEnemyShip(sf::Vector2f position, ShipClass type) {
	// Create a new ship
	std::shared_ptr<EnemyShip> ship = std::make_shared<EnemyShip>();

	// Set up the ship
	ship->setUpShip(type);
	ship->getCannonHandler()->setCooldown(enemyCannonCooldown);
	ship->getMovementHandler()->setTurningSpeed(turningSpeed);
	ship->getMovementHandler()->setEnemySpeedMultiplier(enemySpeedMultiplier);
	ship->getInputHandler()->setFiringDistance(firingDistance);
	ship->getSprite().setPosition(position);

	// Here, the hashmap for the cannonballs is given to each ship. The hashmap is taken
	// from the GlobalHashmapHandler, which you would think wouldnt be necessary since the
	// hashmap is global, but it is necessary because to avoid circular dependencies because
	// the global hashmap handler includes enemy ship which included the SCH.
	ship->getCannonHandler()->setCannonballHashmap(GlobalHashmapHandler::getInstance().getCannonballHashmap());

	// Add the ship to a new ship group and vector, std::move is not needed
	// do after setting up the ship, otherwise the ship will be empty
	std::shared_ptr<ShipGroup> group = std::make_shared<ShipGroup>();
	ship->setGroupID(group->getID());
	group->addShip(ship);
	setShipGroupDestination(group);
	enemyShips.push_back(std::move(ship));
	shipGroups.push_back(std::move(group));
}

void EnemyShipHandler::setShipGroupDestination(std::shared_ptr<ShipGroup> group) {
	sf::Vector2f position = landmasses[std::rand() % landmasses.size()]->getSprite().getPosition();
	group->setDestination(position);
}

bool EnemyShipHandler::isDestinationReached(std::shared_ptr<ShipGroup> shipGroup) const {
	// Get the average position of the ships in the group
	sf::Vector2f averagePosition = shipGroup->getAveragePosition();

	// Calculate the distance between the average position and the target
	float distance = vm::distance(averagePosition, shipGroup->getHeading());

	// If the distance is less than the destination reached distance, return true
	if (distance < destinationReachedDistance) return true;
	return false; // Otherwise, return false
}

void EnemyShipHandler::updateGroupDestination(std::shared_ptr<ShipGroup> group) {
	// Check if nullptr
	if (group == nullptr) {
		std::cout << "Error: Ship group: " << group->getID() << " is nullptr when updating destination" << std::endl;
		return;
	}

	// Check if the destination is reached
	if (isDestinationReached(group)) {
		// Set a new destination
		setShipGroupDestination(group);
	}
}

void EnemyShipHandler::updateGroupsNearPlayer() {
	// Grab nearby ships for the player ship
	std::set<EnemyShip*> nearbyShips = GlobalHashmapHandler::getInstance().getShipHashmap()->findObjectsNearObject(playerShip, maxDetectionDistance);
	// Update all the enemy ships groups near the player ship

	//for (auto& ship : nearbyShips) {

	//	// Grab the ship group ID
	//	int groupID = ship->getGroupID();

	//	// Look for the ship group
	//	auto it = std::find_if(shipGroups.begin(), shipGroups.end(), [groupID](std::shared_ptr<ShipGroup> group) { return group->getID() == groupID; });

	//	// Set the target for the ship group. We dont need to check if there is a group, since the ship should always have a group
	//	//(*it)->setTarget(playerShip->getSprite().getPosition());
	//	(*it)->setInCombat(true);

	//}

	std::set<EnemyShip*> nearbyShipsAudio = GlobalHashmapHandler::getInstance().getShipHashmap()->findObjectsNearObject(playerShip, audioRange);

	// Set all the ships in nearby audio range to play cannon sounds
	for (auto& ship : nearbyShipsAudio) {
		ship->getCannonHandler()->setInAudioRange(true);
	}

	// Set all the ships in nearby audio range to not play cannon sounds
	for (auto& ship : enemyShips) {
		if (nearbyShipsAudio.find(ship.get()) == nearbyShipsAudio.end()) {
			ship->getCannonHandler()->setInAudioRange(false);
		}
	}
}

void EnemyShipHandler::update() {
	// Update battle manager
	battleManager.updateBattles();

	updateGroupsNearPlayer();

	// Update all the enemy ships
	for (auto& enemyShipGroup : shipGroups) {
		updateGroupDestination(enemyShipGroup);
		enemyShipGroup->updateGroup();
	}

	// Ship groups edited by random chance when 2 enemy ship groups are close to each other
	// and will be destroyed when they are far away from each other. By default, there is 1 ship in each group.
	// The ship groups will be used to calculate the flocking behavior of the enemy ships.
	for (auto& enemyShipGroup : shipGroups) {

		// Grab nearby ships, of all of the ships in the group
		std::set<EnemyShip*> nearbyShipsTotal;
		for (auto& ship : enemyShipGroup->getEnemyShips()) {
			std::set<EnemyShip*> nearbyShips = GlobalHashmapHandler::getInstance().getShipHashmap()->findObjectsNearObject(ship.get(), interactionDistance);
			nearbyShipsTotal.insert(nearbyShips.begin(), nearbyShips.end());
		}

		updateShipsAsNotNearbyGroup(enemyShipGroup);

		for (auto& otherShip : nearbyShipsTotal) { // For each nearby ship
			interactWithNearbyShips(enemyShipGroup, otherShip);
		}

		updateGroupCombat(enemyShipGroup, nearbyShipsTotal);
	}

	// If the group size is 0, remove the group
	shipGroups.erase(
		std::remove_if(shipGroups.begin(), shipGroups.end(), [](std::shared_ptr<ShipGroup> group) {
			return group->getEnemyShips().size() == 0;
		}),
		shipGroups.end()
	);
}

void EnemyShipHandler::updateShipsAsNotNearbyGroup(std::shared_ptr<ShipGroup> enemyShipGroup) {
	// Remove ships NOT nearby from the recently interacted with list
	auto& totalShips = enemyShips;
	std::set<EnemyShip*> nearbyShipsTotal;

	// Grab all nearby ships from each ship in the group
	for (auto& ship : enemyShipGroup->getEnemyShips()) {
		std::set<EnemyShip*> nearbyShips = GlobalHashmapHandler::getInstance().getShipHashmap()->findObjectsNearObject(ship.get(), notNearbyAnymoreDistance);
		nearbyShipsTotal.insert(nearbyShips.begin(), nearbyShips.end());
	}

	// Remove all ships that are not nearby from the group
	for (auto& ship : totalShips) {
		// Skip ships that are in the same group
		if (ship->getGroupID() == enemyShipGroup->getID()) continue;
		if (nearbyShipsTotal.find(ship.get()) == nearbyShipsTotal.end()) {
			enemyShipGroup->removeGroupIDInteractedWith(ship->getGroupID());
		}
	}
}

void EnemyShipHandler::interactWithNearbyShips(std::shared_ptr<ShipGroup> enemyShipGroup, EnemyShip* otherShip) {
	// Skip ships that are in the same group
	if (otherShip->getGroupID() == enemyShipGroup->getID()) return;

	// Check if the ship has already been interacted with
	if (enemyShipGroup->isGroupIDInteractedWithRecently(otherShip->getGroupID())) return;
	if (enemyShipGroup->getIsInteracting()) return;

	// Otherwise, roll a coin to see if the ship should be added to the group. 1 is a grouping, 2 is an attack, all other values are no interaction.
	std::uniform_int_distribution<int> dist(0, interactionChance + static_cast<int>(enemyShipGroup->getEnemyShips().size())); // The more ships in the group, the less likely it is to interact (to prevent large groups from becoming too large)
	int interaction = dist(GlobalValues::getInstance().getRandomEngine());

	// Shows if there is interaction. Possible framework for future attack indicator!
	GlobalValues::getInstance().displayText(std::to_string(otherShip->getID()) + ", Interact = " + std::to_string(interaction), otherShip->getSprite().getPosition() + sf::Vector2f(25, 25), (interaction != 1 && interaction != 2) ? sf::Color::White : sf::Color::Red, 20);

	auto otherShipGroup = std::find_if(shipGroups.begin(), shipGroups.end(), [otherShip](std::shared_ptr<ShipGroup> group) { return group->getID() == otherShip->getGroupID(); });

	if (interaction == 1) {
		joinGroups(enemyShipGroup, otherShipGroup->get());
	}
	else if (interaction == 2) {
		battleManager.startBattle(enemyShipGroup, otherShipGroup->get());
	}

	// Add the group ID to the list of interacted with groups
	enemyShipGroup->addGroupIDInteractedWithRecently(otherShip->getGroupID());
}

void EnemyShipHandler::joinGroups(std::shared_ptr<ShipGroup> group1, ShipGroup* group2) {

	// Add all of the ships in the other group to the current group
	for (auto& ship : group2->getEnemyShips()) {
		group1->addShip(ship);
	}
	group2->clearEnemyShips();
	//group2->getEnemyShips().clear();
}

void EnemyShipHandler::updateGroupCombat(std::shared_ptr<ShipGroup> enemyShipGroup, std::set<EnemyShip*> nearbyShipsTotal) {
	// Grab the vector of ships combatting
	auto shipsCombatting = enemyShipGroup->getTargetShips();

	// Remove the ship from the list of ships combatting if it is not nearby
	auto removeCondition = [&](const auto& i) {
		return std::find_if(nearbyShipsTotal.begin(), nearbyShipsTotal.end(), [&](EnemyShip* ship) {
			return ship->getID() == i->getID();
			}) == nearbyShipsTotal.end();
		};

	shipsCombatting.erase(
		std::remove_if(shipsCombatting.begin(), shipsCombatting.end(), removeCondition),
		shipsCombatting.end()
	);

	// If the ship group is not in combat, set the group to not in combat
	if (shipsCombatting.size() == 0) {
		enemyShipGroup->setInCombat(false);
	}
}

void EnemyShipHandler::draw() {
	// Draw all the enemy ship groups
	for (auto& enemyShipGroup : shipGroups) {
		enemyShipGroup->drawGroup();
	}
}