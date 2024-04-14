#include "ShipGroup_PG.h"

using namespace PirateGame;

void ShipGroup::updateGroup() {
	for (auto& ship : ships) {
		ship->update();
		GlobalHashmapHandler::getInstance().getShipHashmap()->updateObjectPosition(ship.get());

		// Calculate the alignment, cohesion, and separation vectors and add them to the destination
		sf::Vector2f resultantVector = calculateAlignment(ship) + calculateCohesion(ship) + calculateSeparation(ship) + calculateGoalVector(ship);
		resultantVector = GlobalValues::getInstance().normalizeVector(resultantVector);

		// Set the travel direction of the ship
		heading = destination + resultantVector;
		ship->getMovementHandler().setDestination(heading);

		// If the ship is in combat, set the target position
		if (inCombat) {
			if (targetShip->getSprite().getPosition() == sf::Vector2f(0, 0)) {
				std::cout << "Error: ShipGroup [" << ID << "] is in combat but has no target position set!" << std::endl;
			}
			else {
				ship->setTargetPosition(targetShip->getSprite().getPosition());
				ship->getMovementHandler().setTargetVelocity(targetVelocity);
				ship->getMovementHandler().setIsActiveTowardsTarget(true);
			}
		}

		// If the health of the ship is almost 0, remove the ship from the group
		if (ship->getHealth() < 0.001f) {
			removeShip(ship);
		}

		// For debugging purposes
		sf::Vector2f pos = sf::Vector2f(ship->getSprite().getPosition().x + 150.f, ship->getSprite().getPosition().y);
		GlobalValues::getInstance().displayText("ID: " + std::to_string(ID), pos, sf::Color::White, 10);
		GlobalValues::getInstance().displayText("Ship group size: " + std::to_string(ships.size()), pos + sf::Vector2f(0, GlobalValues::getInstance().getTextSize()), sf::Color::White, 10);
		GlobalValues::getInstance().displayText("Heading: " + std::to_string(heading.x) + ", " + std::to_string(heading.y), pos + sf::Vector2f(0, 2 * GlobalValues::getInstance().getTextSize()), sf::Color::White, 10);
		if (inCombat) GlobalValues::getInstance().displayText("targetpos: " + std::to_string(targetShip->getSprite().getPosition().x) + ", " + std::to_string(targetShip->getSprite().getPosition().y), pos + sf::Vector2f(0, 3 * GlobalValues::getInstance().getTextSize()), sf::Color::White, 10);
		GlobalValues::getInstance().displayText("inCombat: " + std::to_string(inCombat), pos + sf::Vector2f(0, 4 * GlobalValues::getInstance().getTextSize()), sf::Color::White, 10);
		GlobalValues::getInstance().displayText("Group speed: " + std::to_string(groupSpeed), pos + sf::Vector2f(0, 5 * GlobalValues::getInstance().getTextSize()), sf::Color::White, 10);
		GlobalValues::getInstance().displayText("Ship speed: " + std::to_string(ship->getMovementHandler().getBaseSpeed()), pos + sf::Vector2f(0, 6 * GlobalValues::getInstance().getTextSize()), sf::Color::White, 10);
	}
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
