#pragma once

/// ShipGroup class is used to represent the group of ships in the game.

#include <SFML/Graphics.hpp>
#include <iostream>

#include "EnemyShip_PG.h"
#include "GlobalIDManager_PG.h"
#include "GlobalHashmapHandler.h"

namespace PirateGame {
	class ShipGroup {
	public:
		ShipGroup() { ID = GlobalIDManager::getInstance().getUniqueID(); }
		~ShipGroup() {};

		void updateGroup();
		void drawGroup() {
			for (auto& ship : enemyShips) {
				ship->draw();
			}
		}

		void addShip(std::shared_ptr<EnemyShip> ship) {
			// If this is the first ship or if the ship has a lower speed than the group speed, set the group speed to the ship's speed
			if (enemyShips.size() == 0 || ship->getMovementHandler().getBaseSpeed() < groupSpeed) {
				groupSpeed = ship->getShipProperties().baseSpeed;
				std::cout << "Group speed set to: " << groupSpeed << std::endl;
			}

			ship->setBaseSpeed(groupSpeed);

			enemyShips.push_back(ship);
			std::cout << "Ship added to group. Group size: " << enemyShips.size() << std::endl;
		}

		void removeShip(std::shared_ptr<EnemyShip> ship) {
			// Remove the ship from the hashmap
			GlobalHashmapHandler::getInstance().getShipHashmap()->removeObject(ship.get());

			// Remove the ship from the vector
			enemyShips.erase(std::remove(enemyShips.begin(), enemyShips.end(), ship), enemyShips.end());
		}

		// Setters
		void setHeading(sf::Vector2f heading) { this->destination = heading; }
		void setTarget(sf::Vector2f target) { this->target = target; }
		void setInCombat(bool inCombat) { this->inCombat = inCombat; }
		void addGroupIDInteractedWith(int groupID) { groupIDsInteractedWith.push_back(groupID); }

		// Getters
		std::vector<std::shared_ptr<EnemyShip>>& getEnemyShips() { return enemyShips; }

		int getID() { return ID; }
		bool getInCombat() { return inCombat; }
		bool isGroupIDInteractedWith(int groupID) {
			if (std::find(groupIDsInteractedWith.begin(), groupIDsInteractedWith.end(), groupID) != groupIDsInteractedWith.end()) {
				return true;
			}
			return false;
		}

		sf::Vector2f getAveragePosition() {
			sf::Vector2f averagePosition = sf::Vector2f(0, 0);
			for (auto& ship : enemyShips) {
				averagePosition += ship->getSprite().getPosition();
			}
			averagePosition /= static_cast<float>(enemyShips.size());
			return averagePosition;
		}
		sf::Vector2f getHeading() { return destination; }
	private:
		/// Functions
		// 3 main methods for flocking behavior
		sf::Vector2f calculateAlignment(std::shared_ptr<EnemyShip> ship);
		sf::Vector2f calculateCohesion(std::shared_ptr<EnemyShip> ship);
		sf::Vector2f calculateSeparation(std::shared_ptr<EnemyShip> ship);
		sf::Vector2f calculateGoalVector(std::shared_ptr<EnemyShip> ship);

		// Variables
		float alignmentWeight = 1.f;
		float cohesionWeight = 1.f;
		float separationWeight = 1.f;
		float goalWeight = 1.f;

		float minDistance = 100.f;
		float groupSpeed = 0.f;

		bool inCombat = false;

		sf::Vector2f destination; // The destination of the ship group
		sf::Vector2f heading; // The heading of the ship group
		sf::Vector2f target; // For combat purposes

		// Game objects
		std::vector<std::shared_ptr<EnemyShip>> enemyShips;
		std::vector<int> groupIDsInteractedWith;

		// Unique ID
		int ID = -1;
	};
}