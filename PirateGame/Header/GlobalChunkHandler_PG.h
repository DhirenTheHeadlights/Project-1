#pragma once

/// Map for pirate game

#include <unordered_map>

#include "Chunk_PG.h"
#include "RegionHandler_PG.h"

namespace PirateGame {
	class GlobalChunkHandler {
	public:
		GlobalChunkHandler() {};

		void initializeMap() {
			// Generate the initial chunks
			Chunk initalChunk(std::pair<int, int>(0, 0), chunkSize, cellSize);
			generateSurroundingChunks(initalChunk);
			updateMapBounds();
		}

		void generateRegionCoords(const std::pair<int, int>& chunkCoord) {

		}

		void updateChunks(sf::RenderWindow* window, const sf::Vector2f position, bool debug = false) {
			Chunk& currentChunk = getChunkAtPosition(position);

			// Check if the player has moved to a new chunk
			if (currentChunk.getChunkCoord() != lastChunkCoord) {
				std::cout << "Player has moved to a new chunk: " << currentChunk.getChunkCoord().first << ", " << currentChunk.getChunkCoord().second << std::endl;
				generateSurroundingChunks(currentChunk);
				deleteChunksOutOfRange(currentChunk);

				// Update the map bounds
				updateMapBounds();

				lastChunkCoord = currentChunk.getChunkCoord();
			}

			// Draw the grid for all chunks
			if (debug) {
				for (auto& chunk : chunks) {
					chunk.getMap()->drawGrid(*window);
				}
			}
		}


		// Setters
		void setRenderDistance(int distance) { renderDistance = distance; }

		// Getters
		Chunk& getChunkAtPosition(const sf::Vector2f position) {
			for (auto& chunk : chunks) {
				if (chunk.getMap()->mapContains(position)) {
					return chunk;
				}
			}
			std::cout << "No chunk found at position: " << position.x << ", " << position.y << std::endl;
			static Chunk defaultChunk(std::pair<int, int>(0, 0), chunkSize, cellSize);
			return defaultChunk;
		}
		std::vector<Chunk>& getAllChunks() {
			return chunks;
		}
		RegionHandler getRegionHandler() const { return regionHandler; }
		sf::FloatRect getMapBounds() const { return mapBounds; }

	private:
		// Vector to store 'chunks'
		std::vector<Chunk> chunks;

		// Store the last chunk the player was in
		std::pair<int, int> lastChunkCoord = { 0, 0 };

		void generateSurroundingChunks(Chunk& currentChunk) {
			for (int dx = -renderDistance; dx <= renderDistance; ++dx) {
				for (int dy = -renderDistance; dy <= renderDistance; ++dy) {
					std::pair<int, int> chunkCoord = { currentChunk.getChunkCoord().first + dx, currentChunk.getChunkCoord().second + dy};
					generateChunk(chunkCoord);
				}
			}
		};

		void generateChunk(const std::pair<int, int>& chunkCoord) {
			// Check if the chunk is already generated
			for (auto& chunk : chunks) {
				if (chunk.getChunkCoord() == chunkCoord) {
					return;
				}
			}
			// Generate the chunk
			Chunk newChunk(chunkCoord, chunkSize, cellSize);
			newChunk.setRegionType(regionHandler.generateRegionType(sf::Vector2f(static_cast<float>(chunkCoord.first), static_cast<float>(chunkCoord.second))));
			chunks.push_back(std::move(newChunk));
		}
		
		void deleteChunksOutOfRange(Chunk& currentChunk) {
			/*std::vector<Chunk> chunksToDelete;
			for (auto& chunk : chunks) {
				if (abs(chunk.getChunkCoord().first - currentChunk.getChunkCoord().first) > renderDistance || 
					abs(chunk.getChunkCoord().second - currentChunk.getChunkCoord().second) > renderDistance) {
					chunksToDelete.push_back(chunk);
				}
			}

			for (auto& chunk : chunksToDelete) {
				deleteChunk(chunk);
			}*/
		}

		void deleteChunk(Chunk& chunk) {
			// Check if the chunk exists, then erase.
			std::erase_if(chunks, [&chunk](const Chunk& c) { return c.getChunkCoord() == chunk.getChunkCoord(); });
		}

		void updateMapBounds() {
			for (auto& chunk : chunks) {
				if (chunk.getChunkCoord().first * chunkSize.x < mapBounds.left) {
					mapBounds.left = chunk.getChunkCoord().first * chunkSize.x;
				}
				if (chunk.getChunkCoord().second * chunkSize.y < mapBounds.top) {
					mapBounds.top = chunk.getChunkCoord().second * chunkSize.y;
				}
				if (chunk.getChunkCoord().first * chunkSize.x + chunkSize.x > mapBounds.left + mapBounds.width) {
					mapBounds.width = chunk.getChunkCoord().first * chunkSize.x + chunkSize.x - mapBounds.left;
				}
				if (chunk.getChunkCoord().second * chunkSize.y + chunkSize.y > mapBounds.top + mapBounds.height) {
					mapBounds.height = chunk.getChunkCoord().second * chunkSize.y + chunkSize.y - mapBounds.top;
				}
			}
		}

		// Values
		RegionHandler regionHandler;
		sf::Vector2f chunkSize = sf::Vector2f(5000.f, 5000.f);
		sf::FloatRect mapBounds;
		int cellSize = 100;
		int renderDistance = 2;
		int regionRenderDistance = 10;
	};
}
