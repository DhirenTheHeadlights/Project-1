#pragma once

/// <summary>
/// This class will be used to handle collisions in the game.
/// Each object will be assigned to the hashmap. This will allow 
/// for quick and easy collsion detection.
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include <set>

#include "LandMass_PG.h"
#include "Ship_PG.h"
#include "Map.h"

namespace PirateGame {
	class Hashmap {
	public:
		Hashmap();

		// Add and remove landmass to hashmap
		void addLandMass(LandMass* landmass);
		void removeLandMass(LandMass* landmass);

		// Generate key for hashmap
		std::string generateKey(sf::Vector2f position);

		// Find landmass near player
		std::set<LandMass*> findLandMassNearPlayer(Ship& ship, sf::RenderWindow& window, bool debug = false);
	private:
		// Hashmap
		std::unordered_map<std::string, LandMass*> hashmap;

		// Map
		Map& map = GlobalValues::getInstance().getMap();
	};
}

