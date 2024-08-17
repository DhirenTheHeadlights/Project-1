#pragma once

/// This class handles all the collision detection in the game.

#include <SFML/Graphics.hpp>
#include <iostream>

#include "GlobalContext_PG.h"
#include "GlobalQuadtreeHandler_PG.h"

#include "LandMass_PG.h"
#include "PlayerShip_PG.h"
#include "EnemyShip_PG.h"
#include "Ship_PG.h"

namespace PirateGame {
	class CollisionManager {
	public:
		CollisionManager(GlobalContext& context) : context(context) {}

		void handleCollisions(GlobalQuadtreeHandler* GQH);

		// Setters
		void setLandMasses(std::vector<std::shared_ptr<LandMass>>& landMasses) { this->landMasses = landMasses; }
		void setEnemyShips(std::vector<std::shared_ptr<EnemyShip>>& enemyShips) {this->ships = enemyShips; }
		void setPlayerShip(PlayerShip* playerShip) { this->playerShip = playerShip; }
	
	private:
		// Context
		GlobalContext& context;

		std::vector<std::shared_ptr<LandMass>> landMasses;
		std::vector<std::shared_ptr<EnemyShip>> ships;
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

		std::vector<sf::Sprite> handleShipCollisions(GlobalQuadtreeHandler * GQH, Ship* ship);
		void handleLandMassCollision(Ship* ship, LandMass* landmass, std::vector<LandMass*>& collidingLandMasses);
		void handleShipCollision(Ship* ship1, Ship* ship2, std::vector<Ship*>& collidingShips);
		void handleCannonballCollision(Ship* ship, Cannonball* cannonball, std::vector<Cannonball*>& collidingCannonballs);
		void handleCannonballCollision(Cannonball* c1, Cannonball* c2);
	};
}