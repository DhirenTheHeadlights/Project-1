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
			lastChunk = std::make_shared<Chunk>(std::pair<int, int>(0, 0), chunkSize, cellSize);
			generateSurroundingChunks(lastChunk.get());
		}

		void generateRegionCoords(const std::pair<int, int>& chunkCoord) {

		}

		void updateChunks(sf::RenderWindow* window, const sf::Vector2f position, bool debug = false) {
			std::shared_ptr<Chunk> currentChunk = getChunkAtPosition(position);
			// Check if the player has moved to a new chunk
			if (currentChunk->getChunkCoord() != lastChunk->getChunkCoord()) {
				std::cout << "Player has moved to a new chunk: " << currentChunk->getChunkCoord().first << ", " << currentChunk->getChunkCoord().second << std::endl;
				generateSurroundingChunks(currentChunk.get());
				deleteChunksOutOfRange(currentChunk.get());
			}
			lastChunk = currentChunk;

			// Draw the grid for all chunks
			if (debug) {
				for (auto& chunk : chunks) {
					chunk->getMap()->drawGrid(*window);
				}
			}
		}


		// Setters
		void setRenderDistance(int distance) { renderDistance = distance; }

		// Getters
		std::shared_ptr<Chunk> getChunkAtPosition(sf::Vector2f position) {
			for (auto& chunk : chunks) {
				if (chunk->getMap()->mapContains(position)) {
					return chunk;
				}
			}
			std::cout << "No chunk found at position: " << position.x << ", " << position.y << std::endl;
			return nullptr;
		}
		std::vector<std::shared_ptr<Chunk>> getAllChunks() {
			return chunks;
		}
		RegionHandler getRegionHandler() const { return regionHandler; }

	private:
		// Vector to store 'chunks'
		std::vector<std::shared_ptr<Chunk>> chunks;

		// Store the last chunk the player was in
		std::shared_ptr<Chunk> lastChunk;

		void generateSurroundingChunks(Chunk* currentChunk) {
			for (int dx = -renderDistance; dx <= renderDistance; ++dx) {
				for (int dy = -renderDistance; dy <= renderDistance; ++dy) {
					std::pair<int, int> chunkCoord = { currentChunk->getChunkCoord().first + dx, currentChunk->getChunkCoord().second + dy};
					generateChunk(chunkCoord);
				}
			}
		};

		void generateChunk(const std::pair<int, int>& chunkCoord) {
			// Check if the chunk is already generated
			for (auto& chunk : chunks) {
				if (chunk->getChunkCoord() == chunkCoord) {
					return;
				}
			}
			// Generate the chunk
			std::shared_ptr<Chunk> newChunk = std::make_shared<Chunk>(chunkCoord, chunkSize, cellSize);
			newChunk->setRegionType(regionHandler.generateRegionType(sf::Vector2f(static_cast<float>(chunkCoord.first), static_cast<float>(chunkCoord.second))));
			//grab all enemy ships in newChunks' bounds
			
			chunks.push_back(newChunk);
		}
		
		void deleteChunksOutOfRange(Chunk* currentChunk) {
			std::vector<Chunk*> chunksToDelete;
			for (auto& chunk : chunks) {
				if (abs(chunk->getChunkCoord().first - currentChunk->getChunkCoord().first) > renderDistance || 
					abs(chunk->getChunkCoord().second - currentChunk->getChunkCoord().second) > renderDistance) {
					chunksToDelete.push_back(chunk.get());
				}
			}

			for (auto& chunk : chunksToDelete) {
				deleteChunk(chunk);
			}
		}

		void deleteChunk(Chunk* chunk) {
			// Check if the chunk exists, then erase.
			std::erase_if(chunks, [&](const std::shared_ptr<Chunk>& c) { return c.get() == chunk; });
		}

		// Values
		RegionHandler regionHandler;
		sf::Vector2f chunkSize = sf::Vector2f(5000.f, 5000.f);
		int cellSize = 100;
		int renderDistance = 2;
		int regionRenderDistance = 10;
	};
}
