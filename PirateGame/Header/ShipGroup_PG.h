#pragma once

/// ShipGroup class is used to represent the group of ships in the game.

#include <SFML/Graphics.hpp>
#include <iostream>

#include "EnemyShip_PG.h"

namespace PirateGame {
	class ShipGroup {
	public:
		ShipGroup() {};
		~ShipGroup() {};

		void updateGroup();

		void addShip(std::shared_ptr<EnemyShip> ship) {
			enemyShips.push_back(ship);
		}

		void removeShip(std::shared_ptr<EnemyShip> ship) {
			enemyShips.erase(std::remove(enemyShips.begin(), enemyShips.end(), ship), enemyShips.end());
		}

		// 3 main methods for flocking behavior
		void calculateAlignment(std::shared_ptr<EnemyShip> ship);
		void calculateCohesion(std::shared_ptr<EnemyShip> ship);
		void calculateSeparation(std::shared_ptr<EnemyShip> ship);
		void calculateGoalVector(std::shared_ptr<EnemyShip> ship);

		// Setters

		// Getters

	private:
		// Functions
		
		// Variables
		float alignmentWeight = 1.f;
		float cohesionWeight = 1.f;
		float separationWeight = 1.f;

		sf::Vector2f heading; // The direction the group is moving

		// Game objects
		std::vector<std::shared_ptr<EnemyShip>> enemyShips;
	};
}