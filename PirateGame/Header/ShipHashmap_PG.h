#pragma once

/// This class will contain all of the ships
/// for quick access and collision detection.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include <set>

#include "GlobalValues_PG.h"
#include "PlayerShip_PG.h"
#include "Map.h"

namespace PirateGame {
	class ShipHashmap {
		// A custom hash function for std::pair<int, int>
		struct pair_hash {
			template <class T1, class T2>
			std::size_t operator () (const std::pair<T1, T2>& pair) const {
				return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
			}
		};

	public:
		ShipHashmap() {};

		// Add and remove ship to hashmap
		void addShip(Ship* ship);
		void removeShip(Ship* ship);

		// Generate key for hashmap
		std::pair<int, int> generateKey(sf::Vector2f position);

		// Find ship near other ship
		std::set<Ship*> findShipsNearShip(Ship* ship, bool debug = false);
	private:
		// Hashmap
		std::unordered_map<std::pair<int, int>, Ship*, pair_hash> hashmap;

		// Map
		Map& map = GlobalValues::getInstance().getMap();
	};
}
