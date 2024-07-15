#pragma once

/// This class handles all the collision detection in the game.

#include <SFML/Graphics.hpp>
#include <iostream>

#include "GlobalValues_PG.h"
#include "GlobalQuadtreeHandler.h"
#include "LandMass_PG.h"
#include "PlayerShip_PG.h"
#include "EnemyShip_PG.h"
#include "Ship_PG.h"

namespace PirateGame {
	class CollisionManager {
	public:
		CollisionManager() {};
		~CollisionManager() {};

		void handleCollisions();

		// Setters
		void setLandMasses(std::vector<std::shared_ptr<LandMass>>& landMasses) { this->landMasses = landMasses; }
		void setEnemyShips(std::vector<std::shared_ptr<EnemyShip>>& enemyShips) { 
			for (auto& ship : enemyShips) {
				ships.push_back(ship.get()); // We can save some time by just making a vector of all ships
			}
		}
		void setPlayerShip(PlayerShip* playerShip) { ships.push_back(playerShip); this->playerShip = playerShip; }
	
	private:
		std::vector<std::shared_ptr<LandMass>> landMasses;
		std::vector<Ship*> ships;
		PlayerShip* playerShip = nullptr;

		// Values
		float collisionDamagePerFrame = 10.f;
		float killExp = 10.f;

		bool addedExp = false;

		float nearbyDistanceLandmass = 5000.f;
		float nearbyDistanceShip = 2000.f;
		float nearbyDistanceCannonball = 500.f;

		// Helper functions
		bool pixelPerfectTest(Ship* ship, LandMass* landmass, unsigned alphaLimit = 5);
		bool shipCollisionTest(Ship* ship1, Ship* ship2);

		void handleLandMassCollision(Ship* ship, LandMass* landmass, std::vector<LandMass*>& collidingLandMasses);
		void handleShipCollision(Ship* ship1, Ship* ship2, std::vector<Ship*>& collidingShips);
		void handleCannonballCollision(Ship* ship, Cannonball* cannonball, std::vector<Cannonball*>& collidingCannonballs);
		void handleCannonballCollision(Cannonball* c1, Cannonball* c2);
	};
}