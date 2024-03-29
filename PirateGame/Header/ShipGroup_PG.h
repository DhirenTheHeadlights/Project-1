#pragma once

/// ShipGroup class is used to represent the group of ships in the game.

#include <SFML/Graphics.hpp>
#include <iostream>

#include "EnemyShip_PG.h"
#include "GlobalIDManager_PG.h"

namespace PirateGame {
	class ShipGroup {
	public:
		ShipGroup() { ID = GlobalIDManager::getInstance().getUniqueID(); }
		~ShipGroup() {};

		void updateGroup();

		void addShip(std::shared_ptr<EnemyShip> ship) {
			enemyShips.push_back(ship);
		}

		void removeShip(std::shared_ptr<EnemyShip> ship) {
			enemyShips.erase(std::remove(enemyShips.begin(), enemyShips.end(), ship), enemyShips.end());
		}

		// 3 main methods for flocking behavior
		sf::Vector2f calculateAlignment(std::shared_ptr<EnemyShip> ship);
		sf::Vector2f calculateCohesion(std::shared_ptr<EnemyShip> ship);
		sf::Vector2f calculateSeparation(std::shared_ptr<EnemyShip> ship);
		sf::Vector2f calculateGoalVector(std::shared_ptr<EnemyShip> ship);

		// Setters
		void setHeading(sf::Vector2f heading) { this->heading = heading; }

		// Getters
		std::vector<std::shared_ptr<EnemyShip>>& getEnemyShips() { return enemyShips; }
	private:
		// Functions
		
		// Variables
		float alignmentWeight = 1.f;
		float cohesionWeight = 1.f;
		float separationWeight = 1.f;
		float goalWeight = 1.f;

		float minDistance = 100.f;

		sf::Vector2f heading; // The direction the group is moving

		// Game objects
		std::vector<std::shared_ptr<EnemyShip>> enemyShips;

		// Unique ID
		int ID = -1;
	};
}