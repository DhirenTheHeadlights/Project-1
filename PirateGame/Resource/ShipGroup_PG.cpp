#include "ShipGroup_PG.h"

using namespace PirateGame;

void ShipGroup::updateGroup() {
	for (size_t i = 0; i < ships.size(); i++) {
		std::shared_ptr<EnemyShip> ship = ships[i];
		ship->update();
		GlobalQuadtreeHandler::getInstance().getShipQuadtree()->updateObjectPosition(ship.get());

		// Calculate the alignment, cohesion, and separation vectors and add them to the destination
		sf::Vector2f resultantVector = vm::normalize(calculateAlignment(ship) + calculateCohesion(ship) + calculateSeparation(ship) + calculateGoalVector(ship));

		// If the ship is in combat, set the target position to be the closest enemy ship
		Ship* targetShip = getClosestEnemyShip(ship);
		if (inCombat && targetShip != nullptr) {
			ship->setTargetPosition(targetShip->getSprite().getPosition());
			ship->getMovementHandler()->setActiveTowardsTarget(true);
			ship->getMovementHandler()->setBaseSpeed(combatSpeedMultiplier * groupSpeed);
		}
		else {
			ship->getMovementHandler()->setActiveTowardsTarget(false);
			ship->getMovementHandler()->setBaseSpeed(groupSpeed);
		}

		// If the health of the ship is almost 0, remove the ship from the group
		if (ship->getHealth() < 0.001f) {
			ship->setDead(true);
			removeShip(ship);
		}

		// If any ships in the target ships vector is null or has health less than 0.001f, remove them from the vector
		std::erase_if(targetShips, [](Ship* targetShip) { return targetShip == nullptr || targetShip->getHealth() < 0.001f; });
	}
}

void ShipGroup::drawGroup(bool debug) {
	for (auto& ship : ships) {
		ship->draw();

		/// For debugging purposes
		if (!debug) continue;

		// Check if the shipgroup is near the view, if so, display the shipgroup information
		if (!(vm::distance(ship->getSprite().getPosition(), GlobalValues::getInstance().getWindow()->getView().getCenter()) < 2000.f)) continue;

		sf::Vector2f pos = sf::Vector2f(ship->getSprite().getPosition().x + 150.f, ship->getSprite().getPosition().y);
		GlobalValues::getInstance().displayText("GID: " + std::to_string(ID) + " SID: " + std::to_string(ship->getID()), pos, sf::Color::White, 10);
		GlobalValues::getInstance().displayText("Ship group size: " + std::to_string(ships.size()), pos + sf::Vector2f(0, static_cast<float>(GlobalValues::getInstance().getTextSize())), sf::Color::White, 10);
		GlobalValues::getInstance().displayText("Heading: " + std::to_string(heading.x) + ", " + std::to_string(heading.y), pos + sf::Vector2f(0, 2 * static_cast<float>(GlobalValues::getInstance().getTextSize())), sf::Color::White, 10);
		GlobalValues::getInstance().displayText("Num of target ships: " + std::to_string(targetShips.size()), pos + sf::Vector2f(0, 3 * static_cast<float>(GlobalValues::getInstance().getTextSize())), sf::Color::White, 10);
		std::string targetShipIDs = "Target ship IDs: ";
		for (auto& targetShip : targetShips) {
			targetShipIDs += std::to_string(targetShip->getID()) + ", ";
		}
		GlobalValues::getInstance().displayText(targetShipIDs, pos + sf::Vector2f(0, 4 * static_cast<float>(GlobalValues::getInstance().getTextSize())), sf::Color::White, 10);
		std::string groupIDsInteractedWithStr = "Group IDs interacted with: ";
		for (auto& groupID : groupIDsInteractedWith) {
			groupIDsInteractedWithStr += std::to_string(groupID) + ", ";
		}
		GlobalValues::getInstance().displayText(groupIDsInteractedWithStr, pos + sf::Vector2f(0, 5 * static_cast<float>(GlobalValues::getInstance().getTextSize())), sf::Color::White, 10);
	}
}

Ship* ShipGroup::getClosestEnemyShip(std::shared_ptr<EnemyShip> ship) {
	Ship* closestShip = nullptr;
	float closestDistance = std::numeric_limits<float>::max();

	for (auto& ship : targetShips) {
		float distance = vm::distance(ship->getSprite().getPosition(), ship->getSprite().getPosition());
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
			float distance = vm::distance(ship->getSprite().getPosition(), otherShip->getSprite().getPosition());
			if (distance < minDistance) {
				alignment += otherShip->getMovementHandler()->getVelocity();
				count++;
			}
		}
	}

	if (count > 0) {
		alignment /= static_cast<float>(count);
		alignment = vm::normalize(alignment);
		alignment *= alignmentWeight;
	}

	return alignment;
}

sf::Vector2f ShipGroup::calculateCohesion(std::shared_ptr<EnemyShip> ship) {
	sf::Vector2f cohesion = sf::Vector2f(0, 0);
	int count = 0;

	for (auto& otherShip : ships) {
		if (ship != otherShip) {
			float distance = vm::distance(ship->getSprite().getPosition(), otherShip->getSprite().getPosition());
			if (distance < minDistance) {
				cohesion += otherShip->getSprite().getPosition();
				count++;
			}
		}
	}

	if (count > 0) {
		cohesion /= static_cast<float>(count);
		cohesion -= ship->getSprite().getPosition();
		cohesion = vm::normalize(cohesion);
		cohesion *= cohesionWeight;
	}

	return cohesion;
}

sf::Vector2f ShipGroup::calculateSeparation(std::shared_ptr<EnemyShip> ship) {
	sf::Vector2f separation = sf::Vector2f(0, 0);
	int count = 0;

	for (auto& otherShip : ships) {
		if (ship != otherShip) {
			float distance = vm::distance(ship->getSprite().getPosition(), otherShip->getSprite().getPosition());
			if (distance < 100) {
				separation += otherShip->getSprite().getPosition() - ship->getSprite().getPosition();
				count++;
			}
		}
	}

	if (count > 0) {
		separation /= static_cast<float>(count);
		separation = vm::normalize(separation);
		separation *= separationWeight;
	}

	return separation;
}

sf::Vector2f ShipGroup::calculateGoalVector(std::shared_ptr<EnemyShip> ship) const {
	// Calculate the goal vector from the average position of all the ships
	// and the position of the heading
	sf::Vector2f goalVector = destination - ship->getSprite().getPosition();
	goalVector = vm::normalize(goalVector);
	goalVector *= goalWeight;

	return goalVector;
}
