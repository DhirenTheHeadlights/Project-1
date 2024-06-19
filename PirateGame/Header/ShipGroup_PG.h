#pragma once

/// ShipGroup class is used to represent the group of ships in the game.

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Ship_PG.h"
#include "EnemyShip_PG.h"
#include "GlobalIDManager_PG.h"
#include "GlobalQuadtreeHandler.h"

namespace PirateGame {
	class ShipGroup {
	public:
		ShipGroup() { ID = GlobalIDManager::getInstance().getUniqueID(); }
		~ShipGroup() {};

		void updateGroup();
		void drawGroup(bool debug = false);

		void addShip(std::shared_ptr<EnemyShip> ship) {
			// If this is the first ship or if the ship has a lower speed than the group speed, set the group speed to the ship's speed
			if (ships.size() == 0 || ship->getMovementHandler()->getBaseSpeed() < groupSpeed) {
				groupSpeed = ship->getSpecificShipProperties().baseSpeed * ship->getMovementHandler()->getEnemySpeedMultiplier();
			}

			ship->setBaseSpeed(groupSpeed);
			ship->setGroupID(ID);
			ship->getMovementHandler()->setDestination(destination);

			// Add the ship to the hashmap
			GlobalQuadtreeHandler::getInstance().getShipQuadtree()->addObject(ship.get());

			ships.push_back(ship);
			//std::cout << "Ship added to group. Group size: " << ships.size() << std::endl;
		}

		void removeShip(std::shared_ptr<EnemyShip> ship) {
			// Remove the ship from the hashmap
			GlobalQuadtreeHandler::getInstance().getShipQuadtree()->removeObject(ship.get());

			// Check if the ship was the slowest ship in the group
			if (ship->getMovementHandler()->getBaseSpeed() == groupSpeed) {
				// If it was, find the next slowest ship and set the group speed to that ship's speed
				float newGroupSpeed = 0.f;
				for (auto& otherShip : ships) {
					if (otherShip->getMovementHandler()->getBaseSpeed() < newGroupSpeed) {
						newGroupSpeed = otherShip->getMovementHandler()->getBaseSpeed() * otherShip->getMovementHandler()->getEnemySpeedMultiplier();
					}
				}
				groupSpeed = newGroupSpeed;
			}

			// Remove the ship from the vector
			ships.erase(std::remove(ships.begin(), ships.end(), ship), ships.end());
		}

		// Setters
		void setDestination(sf::Vector2f heading) { 
			destination = heading; 
			for (auto& ship : ships) {
				ship->getMovementHandler()->setDestination(heading);
			}
		}
		void addTarget(Ship* ship) { 
			if (std::find(targetShips.begin(), targetShips.end(), ship) != targetShips.end()) {
				return;
			}
			targetShips.push_back(ship); 
		}
		void clearEnemyShips() { ships.clear(); }
		void setTargetVelocity(sf::Vector2f targetVelocity) { this->targetVelocity = targetVelocity; }
		void setInCombat(bool inCombat) { this->inCombat = inCombat; }
		void setIsInteracting(bool isInteracting) { this->isInteracting = isInteracting; }
		void addGroupIDInteractedWithRecently(int groupID) { groupIDsInteractedWith.push_back(groupID); }
		void removeGroupIDInteractedWith(int groupID) {
			if (std::find(groupIDsInteractedWith.begin(), groupIDsInteractedWith.end(), groupID) == groupIDsInteractedWith.end()) {
				//std::cout << "Error: Group ID [" << groupID << "] not found in groupIDsInteractedWith vector!" << std::endl;
				return;
			}
			groupIDsInteractedWith.erase(std::remove(groupIDsInteractedWith.begin(), groupIDsInteractedWith.end(), groupID), groupIDsInteractedWith.end());
		}

		// Getters
		std::vector<std::shared_ptr<EnemyShip>>& getEnemyShips() { return ships; }
		std::vector<Ship*> getTargetShips() { return targetShips; }

		int getID() const { return ID; }
		bool getInCombat() const { return inCombat; }
		bool getIsInteracting() const { return isInteracting; }
		bool isGroupIDInteractedWithRecently(int groupID) {
			if (std::find(groupIDsInteractedWith.begin(), groupIDsInteractedWith.end(), groupID) != groupIDsInteractedWith.end()) {
				return true;
			}
			return false;
		}

		sf::Vector2f getAveragePosition() {
			sf::Vector2f averagePosition = sf::Vector2f(0, 0);
			for (auto& ship : ships) {
				averagePosition += ship->getSprite().getPosition();
			}
			averagePosition /= static_cast<float>(ships.size());
			return averagePosition;
		}
		sf::Vector2f getHeading() const { return destination; }
		std::vector<int> getGroupIDsInteractedWith() { return groupIDsInteractedWith; }

	private:
		/// Functions
		// 3 main methods for flocking behavior
		sf::Vector2f calculateAlignment(std::shared_ptr<EnemyShip> ship);
		sf::Vector2f calculateCohesion(std::shared_ptr<EnemyShip> ship);
		sf::Vector2f calculateSeparation(std::shared_ptr<EnemyShip> ship);
		sf::Vector2f calculateGoalVector(std::shared_ptr<EnemyShip> ship) const;

		// Combat methods
		Ship* getClosestEnemyShip(std::shared_ptr<EnemyShip> ship);

		// Variables
		float alignmentWeight = 1.f;
		float cohesionWeight = 1.f;
		float separationWeight = 1.f;
		float goalWeight = 1.f;

		float minDistance = 100.f;
		float groupSpeed = 0.f;
		float combatSpeedMultiplier = 0.75f;

		bool inCombat = false;
		bool isInteracting = false;

		sf::Vector2f destination; // The destination of the ship group
		sf::Vector2f heading; // The heading of the ship group
		sf::Vector2f targetVelocity; // For combat purposes
		std::vector<Ship*> targetShips; // For combat purposes

		// Game objects
		std::vector<std::shared_ptr<EnemyShip>> ships;
		std::vector<int> groupIDsInteractedWith;

		// Unique ID
		int ID = -1;
	};
}