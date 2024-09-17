#pragma once

/// Chunk data

#include "Map.h"
#include "Region_PG.h"


namespace PirateGame {
	class Chunk {
	public:
		Chunk(const std::pair<int, int>& chunkCoord, const sf::Vector2f chunkSize, const int cellSize) : chunkCoord(chunkCoord), chunkSize(chunkSize), cellSize(cellSize) {
			map = std::make_unique<Map>();
			map->initialize(static_cast<int>(chunkSize.x), static_cast<int>(chunkSize.y), cellSize, sf::Vector2f(chunkCoord.first * chunkSize.x, chunkCoord.second * chunkSize.y));
		}

		void update() {
			
		}

		/*void removeShip(std::shared_ptr<EnemyShip> ship) {
			enemyShips.erase(std::remove(enemyShips.begin(), enemyShips.end(), ship), enemyShips.end());
		}

		void transferShip(std::shared_ptr<EnemyShip> ship, Chunk& owner) {
			std::shared_ptr<EnemyShip> movedShip = std::move(ship);
			owner.removeShip(ship);
			enemyShips.push_back(ship);
		}*/

		// Getters
		Map* getMap() const { return map.get(); }
		std::pair<int, int> getChunkCoord() const { return chunkCoord; }
		Region getRegionType() const { return regionType; }
		//std::vector<std::shared_ptr<EnemyShip>>& getEnemyShips() { return enemyShips; }

		// Setters
		void setRegionType(const Region regionType) { this->regionType = regionType; }
	private:
		std::unique_ptr<Map> map;
		std::pair<int, int> chunkCoord;
		sf::Vector2f chunkSize;
		int cellSize = 0;
		Region regionType = Region::SandyShores;


		/*std::vector<std::shared_ptr<EnemyShip>> enemyShips;
		std::vector<std::shared_ptr<LandMass>> landmasses;*/



	};
}