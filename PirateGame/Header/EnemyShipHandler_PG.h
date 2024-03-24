#pragma once

/// This class will handle the enemy ships in the game.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "GlobalValues_PG.h"
#include "GlobalHashmapHandler.h"
#include "EnemyShip_PG.h"

namespace PirateGame {
	class EnemyShipHandler {
	public:
		EnemyShipHandler() {};
		~EnemyShipHandler() {
			// Remove all enemy ships
			enemyShips.clear();
			for (auto& ship : enemyShips) {
				GlobalHashmapHandler::getInstance().getShipHashmap()->removeEnemyShip(ship.get());
			}
		};

		// Add and remove enemy ships
		void addEnemyShips(int numShips);

		// Update and draw
		void update();
		void draw();

		// Setters
		void setPlayerShip(PlayerShip* playerShip) { this->playerShip = playerShip; }

		// Getters
		std::vector<std::shared_ptr<EnemyShip>>& getEnemyShips() { return enemyShips; }

	private:
		// helper functions
		void addEnemyShipsToChunk(Map& map, int numShips);

		std::vector<std::shared_ptr<EnemyShip>> enemyShips;
		PlayerShip* playerShip = nullptr;

		// Values
		float enemyCannonCooldown = 2.f;
		float maxDetectionDistance = 1000.f;
		float firingDistance = 1000.f;
		float turningSpeed = 0.1f;
		float enemySpeedMultiplier = 0.5f;
		float minDistBetweenShips = 1000.f;
	};
}