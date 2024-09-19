#pragma once

/// This class contains all quadtrees.
#include "ChunkHandler_PG.h"
#include "QuadtreeTemplate_PG.h"
#include "Landmass_PG.h"
#include "Island_PG.h"
#include "Rock_PG.h"
#include "Shipwreck_PG.h"
#include "EnemyShip_PG.h"
#include "Cannonball_PG.h"

namespace PirateGame {
	namespace QuadtreeHandler {
		// Hashmaps
		inline std::unique_ptr<Quadtree<LandMass>> landmassQuadtree;
		inline std::unique_ptr<Quadtree<Island>> islandQuadtree;
		inline std::unique_ptr<Quadtree<Rock>> rockQuadtree;
		inline std::unique_ptr<Quadtree<Shipwreck>> shipwreckQuadtree;
		inline std::unique_ptr<Quadtree<EnemyShip>> enemyShipQuadtree;
		inline std::unique_ptr<Quadtree<Cannonball>> cannonballQuadtree;

		// Objects
		inline std::vector<std::shared_ptr<LandMass>> landmasses;
		inline std::vector<std::shared_ptr<EnemyShip>> ships;

		inline void updateQuadtrees(const sf::FloatRect updatedBounds) {
			landmassQuadtree->update(updatedBounds);
			enemyShipQuadtree->update(updatedBounds);
			cannonballQuadtree->update(updatedBounds);
		}

		inline void setUpQuadtrees() {
			landmassQuadtree = std::make_unique<Quadtree<LandMass>>(ChunkHandler::getMapBounds());
			islandQuadtree = std::make_unique<Quadtree<Island>>(ChunkHandler::getMapBounds());
			rockQuadtree = std::make_unique<Quadtree<Rock>>(ChunkHandler::getMapBounds());
			shipwreckQuadtree = std::make_unique<Quadtree<Shipwreck>>(ChunkHandler::getMapBounds());
			enemyShipQuadtree = std::make_unique<Quadtree<EnemyShip>>(ChunkHandler::getMapBounds());
			cannonballQuadtree = std::make_unique<Quadtree<Cannonball>>(ChunkHandler::getMapBounds());
		}

		// Setters
		inline void setLandmasses(const std::vector<std::shared_ptr<LandMass>>& newLandmasses) { landmasses = newLandmasses; }
		inline void setShips(const std::vector<std::shared_ptr<EnemyShip>>& newShips) { ships = newShips; }

	};
}