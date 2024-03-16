#pragma once

/// This class will contain all of the landmasses
/// for quick access and collision detection.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include <set>

#include "GlobalValues_PG.h"
#include "GlobalMap_PG.h"

#include "LandMass_PG.h"
#include "Ship_PG.h"
#include "Map.h"

namespace PirateGame {
	class LandMassHashmap {
		// A custom hash function for std::pair<int, int>
		struct pair_hash {
			template <class T1, class T2>
			std::size_t operator () (const std::pair<T1, T2>& pair) const {
				return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
			}
		};


	public:
		LandMassHashmap() {};

		// Add and remove landmass to hashmap
		void addLandMass(LandMass* landmass);
		void removeLandMass(LandMass* landmass);

		// Generate key for hashmap
		std::pair<int, int> generateKey(sf::Vector2f position);

		// Find landmass near player
		std::set<LandMass*> findLandMassNearShip(Ship* ship, bool debug = false);
	private:
		// Hashmap
		std::unordered_map<std::pair<int, int>, LandMass*, pair_hash> hashmap;

		// Map
		Map& map = *GlobalMap::getInstance().getMap();
	};
}

