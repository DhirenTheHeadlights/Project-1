#pragma once

/// This class handles all the collision detection in the game.

#include <SFML/Graphics.hpp>
#include <iostream>

#include "GlobalValues_PG.h"
#include "LandMass_PG.h"
#include "PlayerShip_PG.h"
#include "EnemyShip_PG.h"
#include "Ship_PG.h"
#include "LandmassHashmap_PG.h"
#include "ShipHashmap_PG.h"

namespace PirateGame {
	class CollisionManager {
	public:
		CollisionManager() {};
		~CollisionManager() {};

		void addObjectsToHashmaps();
		void handleCollisions();

		// Setters
		void setLandMasses(std::vector<std::shared_ptr<LandMass>>& landMasses) { this->landMasses = landMasses; }
		void setEnemyShips(std::vector<std::shared_ptr<EnemyShip>>& enemyShips) { this->enemyShips = enemyShips; }
		void setPlayerShips(PlayerShip* playerShip) { this->playerShip = playerShip; }
	
	private:
		std::vector<std::shared_ptr<LandMass>> landMasses;
		std::vector<std::shared_ptr<EnemyShip>> enemyShips;
		PlayerShip* playerShip = nullptr;

		// Hashmaps
		LandMassHashmap landMassHashmap;
		ShipHashmap shipHashmap;

		// Values
		float collisionDamagePerSecond = 4.f;

		// Helper functions
		bool pixelPerfectTest(const sf::Sprite& object1, const sf::Sprite& object2, unsigned alphaLimit = 5);
	};
}