#pragma once

/// This class contains all quadtrees.

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
		void updateQuadtrees(sf::FloatRect updatedBounds) const {
			landmassQuadtree->update(updatedBounds);
			enemyShipQuadtree->update(updatedBounds);
			cannonballQuadtree->update(updatedBounds);
		}

		void setUpQuadtrees(GlobalChunkHandler* GCH) {
			landmassQuadtree = std::make_unique<Quadtree<LandMass>>(GCH->getMapBounds());
			islandQuadtree = std::make_unique<Quadtree<Island>>(GCH->getMapBounds());
			rockQuadtree = std::make_unique<Quadtree<Rock>>(GCH->getMapBounds());
			shipwreckQuadtree = std::make_unique<Quadtree<Shipwreck>>(GCH->getMapBounds());
			enemyShipQuadtree = std::make_unique<Quadtree<EnemyShip>>(GCH->getMapBounds());
			cannonballQuadtree = std::make_unique<Quadtree<Cannonball>>(GCH->getMapBounds());
		}

		// Getters
		Quadtree<LandMass>* getLandMassQuadtree() { return landmassQuadtree.get(); }
		Quadtree<EnemyShip>* getEnemyShipQuadtree() { return enemyShipQuadtree.get(); }
		Quadtree<Cannonball>* getCannonballQuadtree() { return cannonballQuadtree.get(); }
		Quadtree<Island>* getIslandQuadtree() { return islandQuadtree.get(); }
		Quadtree<Rock>* getRockQuadtree() { return rockQuadtree.get(); }
		Quadtree<Shipwreck>* getShipwreckQuadtree() { return shipwreckQuadtree.get(); }

		// Setters
		void setLandmasses(std::vector<std::shared_ptr<LandMass>> landmasses) { this->landmasses = landmasses; }
		void setShips(std::vector<std::shared_ptr<EnemyShip>> ships) { this->ships = ships; }

	private:
		// Hashmaps
		std::unique_ptr<Quadtree<LandMass>> landmassQuadtree;
		std::unique_ptr<Quadtree<Island>> islandQuadtree;
		std::unique_ptr<Quadtree<Rock>> rockQuadtree;
		std::unique_ptr<Quadtree<Shipwreck>> shipwreckQuadtree;
		std::unique_ptr<Quadtree<EnemyShip>> enemyShipQuadtree;
		std::unique_ptr<Quadtree<Cannonball>> cannonballQuadtree;

		// Objects
		std::vector<std::shared_ptr<LandMass>> landmasses;
		std::vector<std::shared_ptr<EnemyShip>> ships;
	};
}