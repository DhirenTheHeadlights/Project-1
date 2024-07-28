#pragma once

/// Chunk data

#include "Map.h"

namespace PirateGame {
	class Chunk {
	public:
		Chunk(const std::pair<int, int>& chunkCoord, sf::Vector2f chunkSize, int cellSize) : chunkCoord(chunkCoord), chunkSize(chunkSize), cellSize(cellSize) {
			map = std::make_unique<Map>();
			map->grid(static_cast<int>(chunkSize.x), static_cast<int>(chunkSize.y), cellSize, sf::Vector2f(chunkCoord.first * chunkSize.x, chunkCoord.second * chunkSize.y));

		}

		// Getters
		Map* getMap() { return map.get(); }
		std::pair<int, int> getChunkCoord() { return chunkCoord; }


	private:
		std::unique_ptr<Map> map;
		std::pair<int, int> chunkCoord;
		sf::Vector2f chunkSize;
		int cellSize;
	};
}