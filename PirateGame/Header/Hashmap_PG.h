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
#include "PlayerShip_PG.h"
#include "Map.h"

namespace PirateGame {
	// A custom hash function for std::pair<int, int>
	struct pair_hash {
		template <class T1, class T2>
		std::size_t operator () (const std::pair<T1, T2>& pair) const {
			return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
		}
	};

	class Hashmap {
	public:
		Hashmap();

		// Add and remove landmass to hashmap
		void addLandMass(LandMass* landmass);
		void removeLandMass(LandMass* landmass);

		// Generate key for hashmap
		std::pair<int, int> generateKey(sf::Vector2f position);

		// Find landmass near player
		std::set<LandMass*> findLandMassNearPlayer(PlayerShip& ship, sf::RenderWindow& window, bool debug = false);
	private:
		// Hashmap
		std::unordered_map<std::pair<int, int>, LandMass*, pair_hash> hashmap;

		// Map
		Map& map = GlobalValues::getInstance().getMap();
	};
}

