#include "ShipGroup_PG.h"

using namespace PirateGame;

void ShipGroup::updateGroup() {
	for (size_t i = 0; i < ships.size(); i++) {
		std::shared_ptr<EnemyShip> ship = ships[i];
		ship->update();
		GlobalHashmapHandler::getInstance().getShipHashmap()->updateObjectPosition(ship.get());

		// Calculate the alignment, cohesion, and separation vectors and add them to the destination
		sf::Vector2f resultantVector = calculateAlignment(ship) + calculateCohesion(ship) + calculateSeparation(ship) + calculateGoalVector(ship);
		resultantVector = GlobalValues::getInstance().normalizeVector(resultantVector);

		// Set the travel direction of the ship
		heading = destination + resultantVector;
		ship->getMovementHandler().setDestination(heading);

		// If the ship is in combat, set the target position to be the closest enemy ship
		Ship* targetShip = getClosestEnemyShip(ship);
		if (inCombat && targetShip != nullptr) {
			ship->setTargetPosition(targetShip->getSprite().getPosition());
			ship->getMovementHandler().setIsActiveTowardsTarget(true);
		}
		else {
			ship->getMovementHandler().setIsActiveTowardsTarget(false);
		}

		// If the health of the ship is almost 0, remove the ship from the group
		if (ship->getHealth() < 0.001f) {
			removeShip(ship);
		}

		// If any ships in the target ships vector is null or has health less than 0.001f, remove them from the vector
		targetShips.erase(std::remove_if(targetShips.begin(), targetShips.end(), [](Ship* ship) { return ship == nullptr || ship->getHealth() < 0.001f; }), targetShips.end());
	}
}

Ship* ShipGroup::getClosestEnemyShip(std::shared_ptr<EnemyShip> ship) {
	Ship* closestShip = nullptr;
	float closestDistance = 999999.f; // Set to a high value

	for (auto& ship : targetShips) {
		float distance = GlobalValues::getInstance().distanceBetweenPoints(ship->getSprite().getPosition(), ship->getSprite().getPosition());
		if (distance < closestDistance) {
			closestDistance = distance; // Update the closest distance
			closestShip = ship;
		}
		// Check if any of the group ships are erroniously in the target ships vector. Compare the IDs
		// as there is a shared pointer in the group ships vector and a raw pointer in the target ships vector
		if (std::find_if(ships.begin(), ships.end(), [ship](std::shared_ptr<Ship> groupShip) { return groupShip->getID() == ship->getID(); }) != ships.end()) {
			std::cout << "Error: Ship [" << ship->getID() << "] is in the target ships vector but is also in the group ships vector! Removing from target ships vector. Wtf are you doing, change the code you idiot." << std::endl;
			targetShips.erase(std::remove(targetShips.begin(), targetShips.end(), ship), targetShips.end());
		}
	}
	return closestShip;
}

sf::Vector2f ShipGroup::calculateAlignment(std::shared_ptr<EnemyShip> ship) {
	sf::Vector2f alignment = sf::Vector2f(0, 0);
	int count = 0;

	for (auto& otherShip : ships) {
		if (ship != otherShip) {
			float distance = GlobalValues::getInstance().distanceBetweenPoints(ship->getSprite().getPosition(), otherShip->getSprite().getPosition());
			if (distance < minDistance) {
				alignment += otherShip->getMovementHandler().getVelocity();
				count++;
			}
		}
	}

	if (count > 0) {
		alignment /= static_cast<float>(count);
		alignment = GlobalValues::getInstance().normalizeVector(alignment);
		alignment *= alignmentWeight;
	}

	return alignment;
}

sf::Vector2f ShipGroup::calculateCohesion(std::shared_ptr<EnemyShip> ship) {
	sf::Vector2f cohesion = sf::Vector2f(0, 0);
	int count = 0;

	for (auto& otherShip : ships) {
		if (ship != otherShip) {
			float distance = GlobalValues::getInstance().distanceBetweenPoints(ship->getSprite().getPosition(), otherShip->getSprite().getPosition());
			if (distance < minDistance) {
				cohesion += otherShip->getSprite().getPosition();
				count++;
			}
		}
	}

	if (count > 0) {
		cohesion /= static_cast<float>(count);
		cohesion -= ship->getSprite().getPosition();
		cohesion = GlobalValues::getInstance().normalizeVector(cohesion);
		cohesion *= cohesionWeight;
	}

	return cohesion;
}

sf::Vector2f ShipGroup::calculateSeparation(std::shared_ptr<EnemyShip> ship) {
	sf::Vector2f separation = sf::Vector2f(0, 0);
	int count = 0;

	for (auto& otherShip : ships) {
		if (ship != otherShip) {
			float distance = sqrt(pow(otherShip->getSprite().getPosition().x - ship->getSprite().getPosition().x, 2)
										+ pow(otherShip->getSprite().getPosition().y - ship->getSprite().getPosition().y, 2));
			if (distance < 100) {
				separation += otherShip->getSprite().getPosition() - ship->getSprite().getPosition();
				count++;
			}
		}
	}

	if (count > 0) {
		separation /= static_cast<float>(count);
		separation = GlobalValues::getInstance().normalizeVector(separation);
		separation *= separationWeight;
	}

	return separation;
}

sf::Vector2f ShipGroup::calculateGoalVector(std::shared_ptr<EnemyShip> ship) {
	// Calculate the goal vector from the average position of all the ships
	// and the position of the heading
	sf::Vector2f goalVector = destination - ship->getSprite().getPosition();
	goalVector = GlobalValues::getInstance().normalizeVector(goalVector);
	goalVector *= goalWeight;

	return goalVector;
}
