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

		// If the ship is in combat, set the target position to be the i target ship in the vector of ships combatting
		// Here, there are 2 scenarios: 1. The targetShipGroup vector is smaller than the shipGroup vector and 2. The targetShipGroup vector is larger than the shipGroup vector
		// For 1, some ships in the group will not have a target ship to attack, so they will attack the first ship in the targetShipGroup vector
		// For 2, nothing really needs to be done, as the ships in the group will attack the ships in the targetShipGroup vector in order
		EnemyShip* targetShip = getClosestEnemyShip(ship);
		if (inCombat) {
			if (targetShips.size() == 0) {
				std::cout << "Error: ShipGroup [" << ID << "] is in combat but has no target ships set! Setting inCombat to false." << std::endl;
				inCombat = false; // Set inCombat to false
				ship->getMovementHandler().setIsActiveTowardsTarget(false);
			}
			else {
				ship->setTargetPosition(targetShip->getSprite().getPosition());
				ship->getMovementHandler().setIsActiveTowardsTarget(true);
			}
		}

		// If the health of the ship is almost 0, remove the ship from the group
		if (ship->getHealth() < 0.001f) {
			removeShip(ship);
		}

		/// For debugging purposes

		// Check if the shipgroup is near the view, if so, display the shipgroup information
		if (!(GlobalValues::getInstance().distanceBetweenPoints(ship->getSprite().getPosition(), GlobalValues::getInstance().getWindow()->getView().getCenter()) < 2000.f)) continue;

		sf::Vector2f pos = sf::Vector2f(ship->getSprite().getPosition().x + 150.f, ship->getSprite().getPosition().y);
		GlobalValues::getInstance().displayText("ID: " + std::to_string(ID), pos, sf::Color::White, 10);
		GlobalValues::getInstance().displayText("Ship group size: " + std::to_string(ships.size()), pos + sf::Vector2f(0, GlobalValues::getInstance().getTextSize()), sf::Color::White, 10);
		GlobalValues::getInstance().displayText("Heading: " + std::to_string(heading.x) + ", " + std::to_string(heading.y), pos + sf::Vector2f(0, 2 * GlobalValues::getInstance().getTextSize()), sf::Color::White, 10);
		if (inCombat) GlobalValues::getInstance().displayText("targetpos: " + std::to_string(targetShip->getSprite().getPosition().x) + ", " + std::to_string(targetShip->getSprite().getPosition().y), pos + sf::Vector2f(0, 3 * GlobalValues::getInstance().getTextSize()), sf::Color::White, 10);
		if (inCombat) GlobalValues::getInstance().displayText("In combat with ship ID: " + std::to_string(targetShip->getID()), pos + sf::Vector2f(0, 4 * GlobalValues::getInstance().getTextSize()), sf::Color::White, 10);
		GlobalValues::getInstance().displayText("Group speed: " + std::to_string(groupSpeed), pos + sf::Vector2f(0, 5 * GlobalValues::getInstance().getTextSize()), sf::Color::White, 10);
		GlobalValues::getInstance().displayText("Ship speed: " + std::to_string(ship->getMovementHandler().getBaseSpeed()), pos + sf::Vector2f(0, 6 * GlobalValues::getInstance().getTextSize()), sf::Color::White, 10);
		GlobalValues::getInstance().displayText("Num of target ships: " + std::to_string(targetShips.size()), pos + sf::Vector2f(0, 7 * GlobalValues::getInstance().getTextSize()), sf::Color::White, 10);
	}

	// If any ships in the target ships vector is null or has health less than 0.001f, remove them from the vector
	targetShips.erase(std::remove_if(targetShips.begin(), targetShips.end(), [](EnemyShip* ship) { return ship == nullptr || ship->getHealth() < 0.001f; }), targetShips.end());
}

EnemyShip* ShipGroup::getClosestEnemyShip(std::shared_ptr<EnemyShip> ship) {
	EnemyShip* closestShip = nullptr;
	float closestDistance = 999999.f; // Set to a high value

	for (auto& ship : targetShips) {
		float distance = GlobalValues::getInstance().distanceBetweenPoints(ship->getSprite().getPosition(), ship->getSprite().getPosition());
		if (distance < closestDistance) {
			closestDistance = distance; // Update the closest distance
			closestShip = ship;
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
