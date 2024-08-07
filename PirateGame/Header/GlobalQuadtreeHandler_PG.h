#pragma once

/// This class will be a singleton that will contain all of the hashmaps for the game.

#include "QuadtreeTemplate_PG.h"
#include "Landmass_PG.h"
#include "Island_PG.h"
#include "Rock_PG.h"
#include "Shipwreck_PG.h"
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

		void updateQuadtrees() {
			landmassQuadtree->update();
			shipQuadtree->update();
			cannonballQuadtree->update();
		}

		// Getters
		Quadtree<LandMass>* getLandMassQuadtree() { return landmassQuadtree.get(); }
		Quadtree<EnemyShip>* getShipQuadtree() { return shipQuadtree.get(); }
		Quadtree<Cannonball>* getCannonballQuadtree() { return cannonballQuadtree.get(); }
		Quadtree<Island>* getIslandQuadtree() { return islandQuadtree.get(); }
		Quadtree<Rock>* getRockQuadtree() { return rockQuadtree.get(); }
		Quadtree<Shipwreck>* getShipwreckQuadtree() { return shipwreckQuadtree.get(); }

		// Setters
		void setLandmasses(std::vector<std::shared_ptr<LandMass>> landmasses) { this->landmasses = landmasses; }
		void setShips(std::vector<std::shared_ptr<EnemyShip>> ships) { this->ships = ships; }

	private:
		// Private Constructor
		GlobalQuadtreeHandler() {};

		// Delete the copy constructor and assignment operator
		GlobalQuadtreeHandler(GlobalQuadtreeHandler const&) = delete;
		GlobalQuadtreeHandler operator=(GlobalQuadtreeHandler const&) = delete;

		// Hashmaps
		std::unique_ptr<Quadtree<LandMass>> landmassQuadtree = std::make_unique<Quadtree<LandMass>>();
		std::unique_ptr<Quadtree<Island>> islandQuadtree = std::make_unique<Quadtree<Island>>();
		std::unique_ptr<Quadtree<Rock>> rockQuadtree = std::make_unique<Quadtree<Rock>>();
		std::unique_ptr<Quadtree<Shipwreck>> shipwreckQuadtree = std::make_unique<Quadtree<Shipwreck>>();
		std::unique_ptr<Quadtree<EnemyShip>> shipQuadtree = std::make_unique<Quadtree<EnemyShip>>();
		std::unique_ptr<Quadtree<Cannonball>> cannonballQuadtree = std::make_unique<Quadtree<Cannonball>>();

		// Objects
		std::vector<std::shared_ptr<LandMass>> landmasses;
		std::vector<std::shared_ptr<EnemyShip>> ships;
	};
}