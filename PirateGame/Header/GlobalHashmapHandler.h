#pragma once

/// This class will be a singleton that will contain all of the hashmaps for the game.

#include "LandmassHashmap_PG.h"
#include "ShipHashmap_PG.h"

namespace PirateGame {
	class GlobalHashmapHandler {
	public:
		// Public method to access the instance
		static GlobalHashmapHandler& getInstance() {
			static GlobalHashmapHandler instance;
			return instance;
		}

		// Getters
		LandMassHashmap* getLandMassHashmap() { return landmassHashmap.get(); }
		ShipHashmap* getShipHashmap() { return shipHashmap.get(); }
	private:
		// Private Constructor
		GlobalHashmapHandler() {};

		// Delete the copy constructor and assignment operator
		GlobalHashmapHandler(GlobalHashmapHandler const&) = delete;
		GlobalHashmapHandler operator=(GlobalHashmapHandler const&) = delete;

		// Hashmaps
		std::unique_ptr<LandMassHashmap> landmassHashmap = std::make_unique<LandMassHashmap>();
		std::unique_ptr<ShipHashmap> shipHashmap = std::make_unique<ShipHashmap>();
	};
}