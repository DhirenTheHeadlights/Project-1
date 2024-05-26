#pragma once

/// This class will be a singleton that will contain all of the hashmaps for the game.

#include "QuadtreeTemplate_PG.h"
#include "Landmass_PG.h"
#include "EnemyShip_PG.h"
#include "Cannonball_PG.h"

namespace PirateGame {
	class GlobalQuadtreeHandler {
	public:
		// Public method to access the instance
		static GlobalQuadtreeHandler& getInstance() {
			static GlobalQuadtreeHandler instance;
			return instance;
		}

		// Getters
		Quadtree<LandMass>* getLandMassHashmap() { return landmassHashmap.get(); }
		Quadtree<EnemyShip>* getShipHashmap() { return shipHashmap.get(); }
		Quadtree<Cannonball>* getCannonballHashmap() { return cannonballHashmap.get(); }
	private:
		// Private Constructor
		GlobalQuadtreeHandler() {};

		// Delete the copy constructor and assignment operator
		GlobalQuadtreeHandler(GlobalQuadtreeHandler const&) = delete;
		GlobalQuadtreeHandler operator=(GlobalQuadtreeHandler const&) = delete;

		// Hashmaps
		std::unique_ptr<Quadtree<LandMass>> landmassHashmap = std::make_unique<Quadtree<LandMass>>();
		std::unique_ptr<Quadtree<EnemyShip>> shipHashmap = std::make_unique<Quadtree<EnemyShip>>();
		std::unique_ptr<Quadtree<Cannonball>> cannonballHashmap = std::make_unique<Quadtree<Cannonball>>();
	};
}