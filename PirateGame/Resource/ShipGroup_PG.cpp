#include "ShipGroup_PG.h"

using namespace PirateGame;

void ShipGroup::updateGroup() {
	for (auto& ship : enemyShips) {
		ship->update();

		// Calculate the alignment, cohesion, and separation vectors
		calculateAlignment(ship);
		calculateCohesion(ship);
		calculateSeparation(ship);
		calculateGoalVector(ship);
	}
}

void ShipGroup::calculateAlignment(std::shared_ptr<EnemyShip> ship) {
	sf::Vector2f alignment = sf::Vector2f(0, 0);
	int count = 0;

	for (auto& otherShip : enemyShips) {
		if (ship != otherShip) {
			float distance = sqrt(pow(otherShip->getSprite().getPosition().x - ship->getSprite().getPosition().x, 2) 
				+ pow(otherShip->getSprite().getPosition().y - ship->getSprite().getPosition().y, 2));
			if (distance < 100) {
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

	ship->getMovementHandler().setDestination(alignment);
}

void ShipGroup::calculateCohesion(std::shared_ptr<EnemyShip> ship) {
	sf::Vector2f cohesion = sf::Vector2f(0, 0);
	int count = 0;

	for (auto& otherShip : enemyShips) {
		if (ship != otherShip) {
			float distance = sqrt(pow(otherShip->getSprite().getPosition().x - ship->getSprite().getPosition().x, 2)
							+ pow(otherShip->getSprite().getPosition().y - ship->getSprite().getPosition().y, 2));
			if (distance < 100) {
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

	ship->getMovementHandler().setDestination(cohesion);
}

void ShipGroup::calculateSeparation(std::shared_ptr<EnemyShip> ship) {
	sf::Vector2f separation = sf::Vector2f(0, 0);
	int count = 0;

	for (auto& otherShip : enemyShips) {
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

	ship->getMovementHandler().setDestination(separation);
}

void ShipGroup::calculateGoalVector(std::shared_ptr<EnemyShip> ship) {
	sf::Vector2f goal = sf::Vector2f(0, 0);
	goal = GlobalValues::getInstance().normalizeVector(sf::Vector2f(100, 100));
	goal *= 1.f;

	ship->getMovementHandler().setDestination(goal);
}