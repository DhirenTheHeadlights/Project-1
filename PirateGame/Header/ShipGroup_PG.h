#pragma once

/// ShipGroup class is used to represent the group of ships in the game.

#include <SFML/Graphics.hpp>

#include "QuadtreeTemplate_PG.h"

#include "Ship_PG.h"
#include "EnemyShip_PG.h"

namespace PirateGame {
	class ShipGroup {
	public:
		ShipGroup(GlobalContext& context) : context(context), ID(context.GIDM->generateID()) {};
		~ShipGroup() {};

		void updateGroup(Quadtree<EnemyShip>* shipQuadtree);
		void drawGroup(bool debug = false) const;

		void addShip(std::shared_ptr<EnemyShip> ship, Quadtree<EnemyShip>* shipQuadtree) {
			// If this is the first ship or if the ship has a lower speed than the group speed, set the group speed to the ship's speed
			if (ships.size() == 0 || ship->getSpecificShipProperties().baseSpeed < groupSpeed) {
				groupSpeed = ship->getSpecificShipProperties().baseSpeed * ship->getMovementHandler()->getEnemySpeedMultiplier();
			}

			ship->getSpecificShipProperties().baseSpeed = groupSpeed;
			ship->setGroupID(ID.get());
			ship->getMovementHandler()->setDestination(destination);

			// Add the ship to the hashmap
			shipQuadtree->addObject(ship.get());

			ships.push_back(ship);
			//std::cout << "Ship added to group. Group size: " << ships.size() << std::endl;
		}

		void removeShip(std::shared_ptr<EnemyShip> ship, Quadtree<EnemyShip>* shipQuadtree) {
			// Remove the ship from the hashmap
			shipQuadtree->removeObject(ship.get());

			// Check if the ship was the slowest ship in the group
			if (ship->getSpecificShipProperties().baseSpeed == groupSpeed) {
				// If it was, find the next slowest ship and set the group speed to that ship's speed
				float newGroupSpeed = 0.f;
				for (auto& otherShip : ships) {
					if (otherShip->getSpecificShipProperties().baseSpeed < newGroupSpeed) {
						newGroupSpeed = otherShip->getSpecificShipProperties().baseSpeed * otherShip->getMovementHandler()->getEnemySpeedMultiplier();
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
		void addGroupIDInteractedWithRecently(ID* groupID) { groupIDsInteractedWith.push_back(groupID); }
		void removeGroupIDInteractedWith(ID* groupID) {
			if (std::find(groupIDsInteractedWith.begin(), groupIDsInteractedWith.end(), groupID) == groupIDsInteractedWith.end()) {
				//std::cout << "Error: Group ID [" << groupID << "] not found in groupIDsInteractedWith vector!" << std::endl;
				return;
			}
			groupIDsInteractedWith.erase(std::remove(groupIDsInteractedWith.begin(), groupIDsInteractedWith.end(), groupID), groupIDsInteractedWith.end());
		}

		// Getters
		std::vector<std::shared_ptr<EnemyShip>>& getEnemyShips() { return ships; }
		std::vector<Ship*> getTargetShips() { return targetShips; }

		ID* getID() const { return ID.get(); }
		bool getInCombat() const { return inCombat; }
		bool getIsInteracting() const { return isInteracting; }
		bool isGroupIDInteractedWithRecently(ID* groupID) {
			if (std::find(groupIDsInteractedWith.begin(), groupIDsInteractedWith.end(), groupID) != groupIDsInteractedWith.end()) {
				return true;
			}
			return false;
		}

		sf::Vector2f getAveragePosition() const {
			sf::Vector2f averagePosition = sf::Vector2f(0, 0);
			for (auto& ship : ships) {
				averagePosition += ship->getSprite().getPosition();
			}
			averagePosition /= static_cast<float>(ships.size());
			return averagePosition;
		}
		sf::Vector2f getHeading() const { return destination; }
		std::vector<ID*> getGroupIDsInteractedWith() { return groupIDsInteractedWith; }

	private:
		// Context
		GlobalContext& context;

		/// Functions
		// 3 main methods for flocking behavior
		sf::Vector2f calculateAlignment(std::shared_ptr<EnemyShip> ship);
		sf::Vector2f calculateCohesion(std::shared_ptr<EnemyShip> ship);
		sf::Vector2f calculateSeparation(std::shared_ptr<EnemyShip> ship);
		sf::Vector2f calculateGoalVector(std::shared_ptr<EnemyShip> ship) const;

		// Combat methods
		Ship* getClosestEnemyShip(std::shared_ptr<EnemyShip> ship);

		// Variables
		float alignmentWeight = context.JSL->getGameData().gameConfig.shipGroupData.shipGroupAlignmentWeight;
		float cohesionWeight = context.JSL->getGameData().gameConfig.shipGroupData.shipGroupCohesionWeight;
		float separationWeight = context.JSL->getGameData().gameConfig.shipGroupData.shipGroupSeparationWeight;
		float goalWeight = 1.f;

		float minDistance = 100.f;
		float groupSpeed = 0.f;
		float combatSpeedMultiplier = context.JSL->getGameData().gameConfig.shipGroupData.shipGroupCombatSpeedMultiplier;

		bool inCombat = false;
		bool isInteracting = false;

		sf::Vector2f destination; // The destination of the ship group
		sf::Vector2f heading; // The heading of the ship group
		sf::Vector2f targetVelocity; // For combat purposes
		std::vector<Ship*> targetShips; // For combat purposes

		// Game objects
		std::vector<std::shared_ptr<EnemyShip>> ships;
		std::vector<ID*> groupIDsInteractedWith;

		// Unique ID
		std::shared_ptr<ID> ID;
	};
}