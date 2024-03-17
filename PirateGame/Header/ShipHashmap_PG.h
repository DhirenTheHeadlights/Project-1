#pragma once

/// This class will contain all of the ships
/// for quick access and collision detection.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <set>

#include "GlobalValues_PG.h"
#include "GlobalMap_PG.h"

#include "PlayerShip_PG.h"
#include "EnemyShip_PG.h"
#include "Map.h"

// This template specialization is used to create a custom hash function for std::pair<int, int>
namespace std {
	template<>
	struct hash<pair<int, int>> {
		size_t operator()(const pair<int, int>& pair) const {
			size_t h1 = hash<int>()(pair.first);
			size_t h2 = hash<int>()(pair.second);
			h1 ^= h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2); // Use a magic number from the golden ratio
			return h1;
		}
	};
}

namespace PirateGame {
	class ShipHashmap {
		// A custom hash function for std::pair<int, int>
		struct pair_hash {
			std::size_t operator()(const std::pair<int, int>& pair) const {
				std::size_t h1 = std::hash<int>()(pair.first);
				std::size_t h2 = std::hash<int>()(pair.second);
				h1 ^= h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2); // Use a magic number from the golden ratio
				return h1;
			}
		};

	public:
		ShipHashmap() {};

		// Add and remove ship to hashmap
		void addEnemyShip(EnemyShip* ship);
		void removeEnemyShip(EnemyShip* ship);

		// Update ship in hashmap
		void updateEnemyShipPosition(EnemyShip* ship);

		// Generate key for hashmap
		std::pair<int, int> generateKey(sf::Vector2f position);

		// Find ship near other ship
		std::set<EnemyShip*> findEnemyShipsNearShip(Ship* ship, float maxDistance = 1000.f, bool debug = false);
	private:
		// Hashmap
		std::unordered_map<std::pair<int, int>, EnemyShip*, pair_hash> hashmap;
		std::unordered_map<EnemyShip*, std::unordered_set<std::pair<int, int>, pair_hash>> reverseHashmap;

		// Map
		Map& map = *GlobalMap::getInstance().getMap();
	};
}
