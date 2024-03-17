#pragma once

/// Map for pirate game

#include "Map.h"

namespace PirateGame {
	class GlobalMap {
	public:
		// Public method to access the instance
		static GlobalMap& getInstance() {
			static GlobalMap instance;
			return instance;
		}

		void setUpMap() {
			map = std::make_unique<Map>();
			map->grid(static_cast<int>(worldMap.x), 
				      static_cast<int>(worldMap.y), 
					  cellSize);
		}

		// Getters
		Map* getMap() { return map.get(); }
		sf::Vector2f getWorldMap() const { return worldMap; }
	private:
		// Private Constructor
		GlobalMap() {};

		// Delete the copy constructor and assignment operator
		GlobalMap(GlobalMap const&) = delete;
		GlobalMap& operator=(GlobalMap const&) = delete;

		// Map
		std::unique_ptr<Map> map;

		// Values
		sf::Vector2f worldMap = sf::Vector2f(100000.f, 100000.f);
		int cellSize = 10;
	};
}