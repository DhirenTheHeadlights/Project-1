#pragma once

/// This class will be a singleton that will contain all of the hashmaps for the game.

#include "HashmapTemplate_PG.h"
#include "Landmass_PG.h"
#include "EnemyShip_PG.h"
#include "Cannonball_PG.h"

namespace PirateGame {
	class GlobalHashmapHandler {
	public:
		// Public method to access the instance
		static GlobalHashmapHandler& getInstance() {
			static GlobalHashmapHandler instance;
			return instance;
		}

		// Getters
		Hashmap<LandMass>* getLandMassHashmap() { return landmassHashmap.get(); }
		Hashmap<EnemyShip>* getShipHashmap() { return shipHashmap.get(); }
		Hashmap<Cannonball>* getCannonballHashmap() { return cannonballHashmap.get(); }
	private:
		// Private Constructor
		GlobalHashmapHandler() {};

		// Delete the copy constructor and assignment operator
		GlobalHashmapHandler(GlobalHashmapHandler const&) = delete;
		GlobalHashmapHandler operator=(GlobalHashmapHandler const&) = delete;

		// Hashmaps
		std::unique_ptr<Hashmap<LandMass>> landmassHashmap = std::make_unique<Hashmap<LandMass>>();
		std::unique_ptr<Hashmap<EnemyShip>> shipHashmap = std::make_unique<Hashmap<EnemyShip>>();
		std::unique_ptr<Hashmap<Cannonball>> cannonballHashmap = std::make_unique<Hashmap<Cannonball>>();
	};
}