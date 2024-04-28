#pragma once

/// This class will handle the enemy ships in the game.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <random>

#include "GlobalValues_PG.h"
#include "GlobalHashmapHandler.h"
#include "ShipGroup_PG.h"

namespace PirateGame {
	class EnemyShipHandler {
	public:
		EnemyShipHandler() {};
		~EnemyShipHandler() {
			// Remove all enemy ships
			enemyShips.clear();
			for (auto& ship : enemyShips) {
				GlobalHashmapHandler::getInstance().getShipHashmap()->removeObject(ship.get());
			}
		};

		// Add and remove enemy ships
		void addEnemyShips(int numShips);

		// Update and draw
		void update();
		void draw();

		// Setters
		void setPlayerShip(PlayerShip* playerShip) { this->playerShip = playerShip; }
		void setLandmasses(std::vector<std::shared_ptr<LandMass>> landmasses) { this->landmasses = landmasses; }

		// Getters
		std::vector<std::shared_ptr<EnemyShip>>& getEnemyShips() { return enemyShips; }

	private:
		// helper functions
		void addEnemyShipsToChunk(Map& map, int numShips);
		void setShipGroupDestination(std::shared_ptr<ShipGroup> group);
		bool isDestinationReached(std::shared_ptr<ShipGroup> shipGroup) const;
		void updateGroupDestination(std::shared_ptr<ShipGroup> group);
		void updateGroupsNearPlayer();

		std::vector<std::shared_ptr<EnemyShip>> enemyShips;
		std::vector<std::shared_ptr<ShipGroup>> shipGroups;
		std::vector<std::shared_ptr<LandMass>> landmasses;
		PlayerShip* playerShip = nullptr;

		// Values
		const float enemyCannonCooldown = 2.f;
		const float maxDetectionDistance = 1000.f;
		const float audioRange = 5000.f;
		const float firingDistance = 1000.f;
		const float turningSpeed = 0.1f;
		const float enemySpeedMultiplier = 0.5f;
		const float minDistBetweenShips = 1000.f;
		const float interactionDistance = 1000.f;

		// Interaction chance is the 1/n chance of a specific interaction; For value x, chance of grouping, attacking, etc. is 1/x
		const int interactionChance = 10;

		// DestinationReachedDistance is arbitrary; ship will choose a new destination at this distance from island. Does not account for diff sizes.
		const float destinationReachedDistance = 500.f;
	};
}